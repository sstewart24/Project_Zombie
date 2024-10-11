#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <vector>

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

// Determinig game speed (player movement, enemy movement, etc.)
const float gameSpeed = 2.0f;

class Wall {
    public:
        float xPos;
        float yPos;
        float xLen;
        float yLen;
		float color[3];
    
    public:
        Wall (float x, float y, float x_l, float y_l) {
            xPos = x;
            yPos = y;
            xLen = x_l;
            yLen = y_l;
		    color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 1.0;
        }

};

class Room {
    public:
		int id;
        std::vector<Wall> walls;

    public:
        Room() {
            //id = 0;          
            //walls.push_back( Wall(50.0f, 100.0f, 50.0f, 50.0f));
            //walls.push_back( Wall(75.0f, 200.0f, 100.0f, 50.0f));
            //walls.push_back( Wall(200.0f, 200.0f, 30.0f, 50.0f));
            //walls.push_back( Wall(400.0f, 50.0f, 200.0f, 100.0f));
            
        }

		Room(int i, std::vector<Wall> w) {
			id = i;
			//while (i < size)
			//{
			walls = w;
			//}

		}

        int checkWall(float newPos[2]) {
            int blocked = 0;
            int i = 0;
            while (!blocked && i < (int)walls.size()) {
                if (newPos[0] > walls[i].xPos && newPos[0] < walls[i].xPos + walls[i].xLen && newPos[1] > walls[i].yPos && newPos[1] < walls[i].yPos + walls[i].yLen) {
                    blocked = 1;
                }
                i++;
            }

            return blocked;
        }

        ~Room() {}
};

class Health {
    public: 
    const int max = 200
    const int min = 0;
    const int z_unaware = 15;
    const int z_alert = 35;
    const int z_hostile = 50;
    const int hbuff_weak = 25;
    const int hbuff_strong = 75;
};

