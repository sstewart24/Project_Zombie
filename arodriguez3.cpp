#include "header.h"
using namespace std;

#define MOVESTEP 1.0f // How fast the zombie moves
//#define FOLLOW_RANGE 100.0f // The following range between zombie and player

extern int checkWall(float*, Room);

int Zcol_count = 0; // For zombie collision countdown

// Vector for Zombies 
std::vector<Zombie> zombies = {
	Zombie(0, 575.0f, 400.0f, 0, 1, 0), // Entrance
	Zombie(1, 75.0f, 100.0f, 0, 1, 0),
	Zombie(2, 575.0f, 200.0f, 0, 1, 0),
	Zombie(3, 75.0f, 150.0f, 0, 1, 0),
	
	Zombie(4, 200.0f, 280.0f, 0, 1, 1), // Office

	Zombie(5, 500.0f, 280.0f, 0, 1, 2), // Hall

	Zombie(6, 555.0f, 350.0f, 0, 1, 4), // Lab
	Zombie(7, 120.0f, 350.0f, 0, 1, 4),
	
	Zombie(8, 200.0f, 260.0f, 0, 1, 5) // Restroom
};

int getVectorSize()
{ // function for Sophia
	return zombies.size(); 
}

Zombie getZombies(int index)
{ // function for Sophia
	return zombies[index];
}

// Function to get a random direction
int getRandomDirection()
{
    return rand() % 4; // Returns a random integer between 0 and 3
}

bool Zcollision(float newPos[2], const Zombie& zombie)
{ // newPos is the other zombie
	for (size_t i = 0; i < zombies.size(); i++) {
		if (zombies[i].id != zombie.id) {
			// Calculate the distance between the zombie and other zombies
			float x = newPos[0] - zombies[i].pos[0];
			float y = newPos[1] - zombies[i].pos[1];
			float distance = sqrt(x * x + y * y);
			
			// If the distance is smaller than the threshold, it is a collision
			if (distance < 18.0f) { // the threshold is 18.0f (2 * 9.0f)
				Zcol_count++;
				if (!zombie.alive) {
					return false;
				}
				if (distance == 0.0f) {
					return false; // No collision
				}
				if (Zcol_count == 0) {
					return true;
				}
				if (Zcol_count >= 1 && Zcol_count < 30) {
					//printf("Counting: %i\n", count);
					return false;
				}
				if (Zcol_count == 30) {
					Zcol_count = 0;
					return true;
				}
                return true; // Collission
			}
		}
	}
	return false; // No collision
}

bool Zfollow(Zombie& zombie, Player& player, Room current)
{
	float follow_range = 100.0f;
	float Ppos[2] = {player.pos[0], player.pos[1]}; 	// Player position
	float Zpos[2] = {zombie.pos[0], zombie.pos[1]}; 	// Zombie position

	// Calculate the distance between the zombie and the player
	float dx = Ppos[0] - Zpos[0];
	float dy = Ppos[1] - Zpos[1];
	float distance = sqrt(dx * dx + dy * dy);

	// If the player is shown and zombie is alive, follow
	if (player.shown && zombie.alive) {
		// If the zombie was following the player and the player is out of range
		if (zombie.following && distance >= follow_range) {
			zombie.count = 0;			// reset count
			zombie.waiting = 1;			// reset waiting
			zombie.following = false;	// stopped following
			//printf("Counter reset\n");
			return false;
		}
		// If the zombie is within the range of the player, follow the player
		if (distance < follow_range) {
			if(!zombie.waiting) {
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

				}
				zombie.following = true; 	// marked as following
				return true; 				// Zombie follows player
			} else {
				zombie.count++;
				if (zombie.count > 1 && zombie.count < 30) {
					//printf("Counting: %i\n", zombie.count);
					return false;
				}
				if (zombie.count == 30) {
					//printf("Count is now: %i\n", zombie.count);
					zombie.waiting = 0;
				}
			}
		}
		if (zombie.waiting && distance > follow_range) {
			// Resets timer when player leaves range
			zombie.count = 0;
			//printf("Out of range, Counter reset\n");
		}
	}
	else if (!player.shown && zombie.following) { 
		// Resets everything when player hides
		zombie.count = 0;
		zombie.waiting = 1;
		zombie.following = false;
		//printf("Counter reset\n");
	}
	return false; // Zombie does not follow player
}

void Zroam(Zombie& zombie, Room current)
{
    // Float moveStep = 2.0f; // how much to move each update
    float newPos[2] = {zombie.pos[0], zombie.pos[1]};
	
    // Update position based on current direction
    switch (zombie.direction) {
        case 0: // Moving up
			//Zwait();
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

	} else if (zombie.count != 0) {
		// Check if zombie is suspicious of player, stop moving
		zombie.moveDistance = 0;
		//printf("Zombie paused\n");
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
				if (!Zfollow(zombies[i], player, current)) {
					Zroam(zombies[i], current);
				}
				
				//draws the zombies
				glPushMatrix();
				glTranslatef(zombies[i].pos[0], zombies[i].pos[1], 0.0f);
				glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
				float size = 9.0f; // size of zombie
				glBegin(GL_QUADS);
					if (Zfollow(zombies[i], player, current)) {
						// When following player (red)
						glColor3f(1.0f, 0.0f, 0.0f);
					} else if (zombies[i].count > 0) {
						// When Zombie is suspicious of player (blue)
						glColor3f(0.0f, 0.0f, 1.0f);
					} else {
						// When Zombie is just roaming (green)
						glColor3f(0.0f, 1.0f, 0.0f);
					}
					glVertex3f(size, size, 0.0f); 	// top right
					glVertex3f(size, -size, 0.0f); 	// bottom right
					glVertex3f(-size, -size, 0.0f); // bottom left
					glVertex3f(-size, size, 0.0f); 	// top left
				glEnd();

				glBegin(GL_POINTS);
				glVertex2f(0.0f, 0.0f);
				glEnd();
				glPopMatrix();
            }
		}
	}
}
