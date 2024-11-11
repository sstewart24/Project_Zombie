/*=================================================================
* Adrian's source file
*
* Currently:
*	-Zombie class
*
*	-Vector for Zombies
*
*	-Int function getRandomDirection to get a random direction for zombies
*	
*	-externs checkWall function from Mathew's source file for zombie hitting walls
*
*	-A Bool function called Zcollision to check for collision between Zombies
*
*	-Void function Zroam so zombies can roam around the rooms
*
*	-Void renderZombie function to render Zombies into rooms 
*	and have them in different positions depending on what room they're in.
*
*
* TO-DO:
*	1)Make Zombie follow player
*
*	2)Make Zombie follow player within a range
*
*
*
*
*
*
*
*====================================================================*/
#include "header.h"


//Zombie Class
class Zombie { 
    public: //Access specifier
	int id; // Identification of the zombie
        int room; // Which room it is in
        int xPos, yPos;
	Vec pos;
	float color[3];

	float moveDistance; // Distance moved so far
    	float maxDistance; // Maximum distance before turning
	int direction; // 0: up, 1: right, 2: down, 3: left

    public:
        //Zombie Constructor
        Zombie(int idZ, float x, float y, int r) 
		: moveDistance(0.0f), maxDistance(100.0f), direction(1) { //member initializer list
            	id = idZ;
		pos[0] = x;
		pos[1] = y;
            	room = r;
		color[0] = 0.0f;
		color[1] = 1.0f;
		color[2] = 0.0f;
        }

	~Zombie() {}
};

//Vector for Zombies 
std::vector<Zombie> zombies = {
	Zombie(0, 575.0f, 400.0f, 0),
	Zombie(1, 75.0f, 100.0f, 0),
	Zombie(2, 575.0f, 200.0f, 0),
	Zombie(3, 75.0f, 50.0f, 0),
	Zombie(0, 575.0f, 300.0f, 1),
	Zombie(1, 375.0f, 200.0f, 1)
};

// Function to get a random direction
int getRandomDirection() {
    return rand() % 4; // Returns a random integer between 0 and 3
}

extern int checkWall(float*, Room);

bool Zcollision(float newPos[2], const Zombie& zombie) {
	for (size_t i = 0; i < zombies.size(); i++) {
		if (zombies[i].id != zombie.id) {
			// Calculate the distance between the zombie and other zombies
			float x = newPos[0] - zombies[i].pos[0];
			float y = newPos[1] - zombies[i].pos[1];
			float distance = std::sqrt(x * x + y * y);

			// if the distance is smaller than the threshold, it is a collision
			if (distance < 18.0f) { // the threshold is 18.0f because of the zombies size in render (2 * 9.0f)
				return true;
			}
		}
	}
	return false;
}

void Zroam(Zombie& zombie, Room current) {
    float moveStep = 2.0f; // how much to move each update
    float newPos[2] = {zombie.pos[0], zombie.pos[1]};
	
    // Update position based on current direction
    switch (zombie.direction) {
        case 0: // Moving up
            newPos[1] += moveStep;
            break;
        case 1: // Moving right
            newPos[0] += moveStep;
            break;
        case 2: // Moving down
            newPos[1] -= moveStep;
            break;
        case 3: // Moving left
            newPos[0] -= moveStep;
            break;
    }

	// Update the distance moved
    zombie.moveDistance += moveStep;

    // Check if max distance has been reached to turn around
    if (zombie.moveDistance >= zombie.maxDistance) {
        zombie.direction = getRandomDirection(); // change direction randomly
        zombie.moveDistance = 0; // Reset distance moved

    } else if (checkWall(newPos, current)) {
        // Collision detected with wall, turn around (180 degrees)
        zombie.direction = getRandomDirection(); 
        zombie.moveDistance = 0; 

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
void renderZombie(Room current) 
{
	//uses for loop to make zombies based on the size of the zombie vector
	for (size_t i = 0; i < zombies.size(); i++) {
		
		//if the zombies room # matches the current room id, draw the zombie
		if (zombies[i].room == current.id) {
			Zroam(zombies[i], current);

			//draws the zombies
			glPushMatrix();
			glTranslatef(zombies[i].pos[0], zombies[i].pos[1], 0.0f);
			glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
			float size = 9.0f; //size of zombie
			glBegin(GL_QUADS);
				glColor3f(0.0f, 1.0f, 0.0f); //green color
				glVertex3f(size, size, 0.0f); //top right
				glVertex3f(size, -size, 0.0f); //bottom right
				glVertex3f(-size, -size, 0.0f); //bottom left
				glVertex3f(-size, size, 0.0f); //top left
			glEnd();

			glBegin(GL_POINTS);
			glVertex2f(0.0f, 0.0f);
			glEnd();
			glPopMatrix();
		}
	}
}
