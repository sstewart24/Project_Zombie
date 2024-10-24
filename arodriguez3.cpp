/*
* Adrian's source file
*
* Currently:
*	render Zombies into rooms and have them in different positions
*	depending on what room they're in.
*
*
*
*
*/

#include "header.h"

//Vector for Zombies 
/*
std::vector<Zombie> zombies = {
	Zombie(0, 575.0f, 400.0f, 0),
	Zombie(1, 75.0f, 100.0f, 0),
	Zombie(0, 575.0f, 400.0f, 1),
	Zombie(1, 375.0f, 200.0f, 1)
};
*/

//Array for Zombies
Zombie zombies[] = {
	Zombie(0, 575.0f, 400.0f, 0),
	Zombie(1, 75.0f, 100.0f, 0),
	Zombie(0, 575.0f, 400.0f, 1),
	Zombie(1, 375.0f, 200.0f, 1)
};

//Drawing/Rendering Zombies
void renderZombie(int numZombies, Room current) 
{
	for (int i = 0; i < numZombies; i++) {
		if (zombies[i].room == current.id) {
			glPushMatrix();
			glTranslatef(zombies[i].pos[0], zombies[i].pos[1], zombies[i].pos[2]);
			glRotatef(zombies[i].angle, 0.0f, 0.0f, 1.0f);
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
	}
}

//int Zcollision() {}
