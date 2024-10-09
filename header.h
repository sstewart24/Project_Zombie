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

class Door {
public:
    //int id;
    float doorPos[2];
    float size[2];
    int toRoom;
    float color[3];
public:
    Door (float x, float y, float x_l, float y_l, int rID) {
        doorPos[0] = x;
        doorPos[1] = y;
        size[0] = x_l;
        size[1] = y_l;
        toRoom = rID;
        color[0] = 0.1f;
        color[1] = color[2] = 0.0f;
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

	Room(int i, std::vector<Wall> w) {
		id = i;
		walls = w;
    }

    ~Room() {}
};
