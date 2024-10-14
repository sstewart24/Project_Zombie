#include "header.h"

/*
Make zombies that roam around the room in a random way, 
look at asteroids and how it randomly moves around the room
*/

void renderZombie(Zombie zombie) //Drawing/Rendering Zombies
{
	glPushMatrix();
	glTranslatef(zombie.pos[0], zombie.pos[1], zombie.pos[2]);
	glRotatef(zombie.angle, 0.0f, 0.0f, 1.0f);
	float size = 9.0f;
	glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
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

const int MAX_ZOMBIES = 2; //number of zombies
Zombie zombies[MAX_ZOMBIES];

void init_zombies(Zombie zombies[], int numZombies) { //function for spawning zombies
	for (int i = 0; i < numZombies; ++i) {
		zombies[i].pos[0] = 100.0f + (i * 50.0f);
		zombies[i].pos[1] = 200.0f;
		zombies[i].angle = 10.0f;
	}

}

//void roam_zombies() {}

//extern Zombie zombie;

