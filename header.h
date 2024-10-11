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

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2];

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

class Door {
public:
    //int id;
    float xPos;
    float yPos;
    float xLen;
    float yLen;
    int toRoom;
    float color[3];
public:
    Door (float x, float y, float x_l, float y_l, int rID) {
        xPos = x;
        yPos = y;
        xLen = x_l;
        yLen = y_l;
        toRoom = rID;
        color[0] = 0.647059f;
        color[1] = color[2] = 0.164706f;
    }

};

class Room {
public:
	int id;
    std::vector<Wall> walls;
    std::vector<Door> doors;

public:
    Room() {
        //id = 0;          
        //walls.push_back( Wall(50.0f, 100.0f, 50.0f, 50.0f));
        //walls.push_back( Wall(75.0f, 200.0f, 100.0f, 50.0f));
        //walls.push_back( Wall(200.0f, 200.0f, 30.0f, 50.0f));
        //walls.push_back( Wall(400.0f, 50.0f, 200.0f, 100.0f));            
    }

	Room(int i, std::vector<Wall> w, std::vector<Door> d) {
		id = i;
		walls = w;
        doors = d;
    }

    ~Room() {}
};

class Zombie { //Zombie Class
    public: //Access specifier
		int xPos, yPos;
		Vec pos;
	    Vec dir;
	    float angle;
	    float color[3];

    public:
        Zombie() { //Zombie Constructor 
			xPos = 640;
			yPos = 480;
			pos[0] = (Flt)(xPos/2);
			pos[1] = (Flt)(yPos/2);
			pos[2] = 0.0f;
			VecZero(dir);
			angle = 0.0;
			
			//xPos = x;
			//yPos = y;
			//dir = d;
			//angle = a;
			
		    color[0] = 0.0f;
			color[1] = 1.0f;
			color[2] = 0.0f;
        }

		~Zombie() {}
};
