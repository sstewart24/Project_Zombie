/*=================================================================
 * Adrian's source file
 *
 * Updated: 11/25/2024
 *
 * Currently:
 *	- Defines for Zroam and Zfollow functions
 *
 *	- Extern of checkWall function from Mathew's source file 
 *	  for zombie hitting walls
 *
 *	- Vector for Zombies
 *
 *	- int function that is made for Sophia to get the size of the Zombie vector
 *
 *	- Another function that is made for Sophia to work on player to zombie collision
 *
 *	- Int function getRandomDirection to get a random direction for zombies
 *	
 *	- A Bool function called Zcollision to check for collision between Zombies
 *
 *	- A Bool function called Zfollow to check if the zombie or player is at
 *	  a certain range, the zombie will follow the player, also calling checkWall
 * 	  so Zombies do not go through walls while following	
 *
 *	- Void function Zroam so zombies can roam around the rooms
 *
 *	- Void renderZombie function to render Zombies into rooms 
 *	  and have them in different positions depending on what room they're in.
 *
 * TO-DO:
 *	1) Give Zombie a delay before following player
 *
 *	2) Maybe give zombie another delay before switching directions
 *
 *	3) Make zombie dissapear when they are killed with axe
 *====================================================================*/
#include "header.h"
using namespace std;

#define MOVESTEP 1.0f // How fast the zombie moves
//#define FOLLOW_RANGE 100.0f // The following range between zombie and player

extern int checkWall(float*, Room);

// Vector for Zombies 
std::vector<Zombie> zombies = {
	Zombie(0, 575.0f, 400.0f, 0, 1, 0),
	Zombie(1, 75.0f, 100.0f, 0, 1, 0),
	Zombie(2, 575.0f, 200.0f, 0, 1, 0),
	Zombie(3, 75.0f, 150.0f, 0, 1, 0),
	//Zombie(4, 575.0f, 300.0f, 0, 1, 1),
	Zombie(4, 200.0f, 280.0f, 0, 1, 1),

	Zombie(5, 500.0f, 280.0f, 0, 1, 2),

	Zombie(6, 555.0f, 350.0f, 0, 1, 4),
	Zombie(7, 120.0f, 350.0f, 0, 1, 4)
};

int getVectorSize() { // function for Sophia
	return zombies.size(); 
}

Zombie getZombies(int index) { // function for Sophia
	return zombies[index];
}

// Function to get a random direction
int getRandomDirection() {
    return rand() % 4; // Returns a random integer between 0 and 3
}

void Zwait() {
    time_t start, current; // start time and current time
    start = time(NULL);

    while (1) {
		current = time(NULL);
		if (current - start >= 1) {
			break;
		}
    }
}

bool Zcollision(float newPos[2], const Zombie& zombie) { // newPos is the other zombie
	for (size_t i = 0; i < zombies.size(); i++) {
		if (zombies[i].id != zombie.id) {
			// Calculate the distance between the zombie and other zombies
			float x = newPos[0] - zombies[i].pos[0];
			float y = newPos[1] - zombies[i].pos[1];
			float distance = sqrt(x * x + y * y);
			
			// If the distance is smaller than the threshold, it is a collision
			if (distance < 18.0f) { // the threshold is 18.0f because of the zombies size in render (2 * 9.0f)
				if (!zombie.alive) {
					return false;
				}
				if (distance <= 0.0f) {
					return false; // No collision
				}
                return true; // Collission
			}

		}
	}
	return false; // No collision
}

bool Zfollow(Zombie& zombie, Player& player, Room current) {
	//static int count = 0;
	float follow_range = 100.0f;
	float Ppos[2] = {player.pos[0], player.pos[1]}; 	// Player position
	float Zpos[2] = {zombie.pos[0], zombie.pos[1]}; 	// Zombie position

	// Calculate the distance between the zombie and the player
	float dx = Ppos[0] - Zpos[0];
	float dy = Ppos[1] - Zpos[1];
	float distance = sqrt(dx * dx + dy * dy);

	// If the player is shown and zombie is alive, follow
	if (player.shown && zombie.alive) {
		// If the zombie is within the range of the player, follow the player
		if (distance < follow_range) {
			//Zwait();
			// Calculate the direction towards the player
			// Normalize the direction vector (dx, dy)
			float Xdir = dx / distance;
			float Ydir = dy / distance;

			// Update zombie position toward the player
			float NewZpos[2];
			NewZpos[0] = zombie.pos[0] + MOVESTEP * Xdir;
			NewZpos[1] = zombie.pos[1] + MOVESTEP * Ydir;

			// Checks to see if the Zombie is colliding with a wall
			if (!checkWall(NewZpos, current)) {
				zombie.pos[0] = NewZpos[0];
				zombie.pos[1] = NewZpos[1];

			} else {
				// Zombie will not move because checkWall with return blocked
			}


			return true; // Zombie follows player
		}
	}
		
	return false; // Zombie does not follow player
}

void Zroam(Zombie& zombie, Room current) {
    // Float moveStep = 2.0f; // how much to move each update
    float newPos[2] = {zombie.pos[0], zombie.pos[1]};
	
    // Update position based on current direction
    switch (zombie.direction) {
        case 0: // Moving up
            newPos[1] += MOVESTEP;
			zombie.angle = 360.0f;
            break;
        case 1: // Moving right
            newPos[0] += MOVESTEP;
			zombie.angle = 270.0f;
            break;
        case 2: // Moving down
            newPos[1] -= MOVESTEP;
			zombie.angle = 180.0f;
            break;
        case 3: // Moving left
            newPos[0] -= MOVESTEP;
			zombie.angle = 90.0f;
            break;
    }

	// Update the distance moved
    zombie.moveDistance += MOVESTEP;

    // Check if max distance has been reached to turn around Zombie
    if (zombie.moveDistance >= zombie.maxDistance) {
        zombie.direction = getRandomDirection(); 	// change direction randomly
        zombie.moveDistance = 0; 					// Reset distance moved

    } else if (checkWall(newPos, current)) {
        // Collision detected with wall, Zombie turns around
        zombie.direction = getRandomDirection(); 
        zombie.moveDistance = 0; 
		return;

    } else if (Zcollision(newPos, zombie)) {
		// Collision detected with another zombie, turn around
        zombie.direction = getRandomDirection();
        zombie.moveDistance = 0;
        return; 

	} else {
        // Update position only if there's no collision
        zombie.pos[0] = newPos[0];
        zombie.pos[1] = newPos[1];
    }
}

//Drawing/Rendering Zombies
void renderZombie(Room current, Player player) 
{
	// Uses for loop to make zombies based on the size of the zombie vector
	for (size_t i = 0; i < zombies.size(); i++) {
		
		// If the zombies room # matches the current room id, draw the zombie
		if (zombies[i].room == current.id) {
            if (zombies[i].alive == 1) {
				if(!Zfollow(zombies[i], player, current)) {
					Zroam(zombies[i], current);
				}
				
				//draws the zombies
				glPushMatrix();
				glTranslatef(zombies[i].pos[0], zombies[i].pos[1], 0.0f);
				glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
				float size = 9.0f; // size of zombie
				glBegin(GL_QUADS);
					if (zombies[i].angle == 360.0f) {
						glColor3f(0.0f, 0.0f, 1.0f); 	// blue color

					}else {
						glColor3f(0.0f, 1.0f, 0.0f); 	// green color
					}
					glVertex3f(size, size, 0.0f); 		// top right
					glVertex3f(size, -size, 0.0f); 		// bottom right
					glVertex3f(-size, -size, 0.0f); 	// bottom left
					glVertex3f(-size, size, 0.0f); 		// top left
				glEnd();

				glBegin(GL_POINTS);
				glVertex2f(0.0f, 0.0f);
				glEnd();
				glPopMatrix();
            		}
		}
	}
}
