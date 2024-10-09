#include "header.h"

/*
Make zombies that roam around the room in a random way, 
look at asteroids and how it randomly moves around the room
*/

class Zombie {
    public:
        float xPos, yPos;
	    float dir;
	    float angle;
	    float color[3];

    public:
        Zombie(float x, float y, float d, float a) {
			xPos = x;
			yPos = y;
			dir = d;
			angle = a;
		    color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 0.0;
        }
};

/*
//Goal: Zombie moves in a random direction on the screen
float xmove, ymove = rnd();

//Check for collision with window edges for zombies
if (g.zombie.xPos < 0.0) {
	g.zombie.xPos += (float)gl.xres;
}
else if (g.zombie.xPos > (float)gl.xres) {
	g.zombie.Xpos -= (float)gl.xres;
}
else if (g.zombie.yPos < 0.0) {
	g.zombie.yPos += (float)gl.yres;
}
else if (g.zombie.yPos > (float)gl.yres) {
	g.zombie.yPos -= (float)gl.yres;
}
*/