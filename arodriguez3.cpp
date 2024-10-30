/*
* Adrian's source file
*
* Currently:
*	-Zombie class
*
*	-Vector for Zombies
*	
*	-render Zombies into rooms and have them in different positions
*	depending on what room they're in.
*
*
*
*
*/
#include "header.h"

//number of max Zombies
//#define MAX_ZOMBIES 10

//Zombie Class
class Zombie { 
    public: //Access specifier
	int id; // Identification of the zombie
        int room; // Which room it is in
        int xPos, yPos;
	Vec pos;
	float color[3];

    public:
        //Zombie Constructor
        Zombie(int idZ, float x, float y, int r) {
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
	Zombie(0, 575.0f, 300.0f, 1),
	Zombie(1, 375.0f, 200.0f, 1)
};

//Drawing/Rendering Zombies
void renderZombie(Room current) 
{
	//uses for loop to make zombies based on the size of the zombie vector
	for (size_t i = 0; i < zombies.size(); i++) {
		
		//if the zombies room # matches the current room id, draw the zombie
		if (zombies[i].room == current.id) {

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

//int Zcollision() {}
extern int checkWall(float*, Room);

//float Zroam(float Pposition[2]) {
//	return 0;
//}
