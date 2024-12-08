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
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <thread>

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
//const double physicsRate = 1.0 / 60.0;
//const double oobillion = 1.0 / 1e9;
//extern struct timespec timeStart, timeCurrent;
//extern struct timespec timePause;
//extern double physicsCountdown;
//extern double timeSpan;
//extern double timeDiff(struct timespec *start, struct timespec *end);
//extern void timeCopy(struct timespec *dest, struct timespec *source);
class Timers {
public:
    double physicsCountdown = 0.0;
    double timeSpan=0.0;
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
    struct timespec timePause;
	struct timespec walkTime;
	Timers() {
		physicsRate = 1.0 / 60.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
};
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

#ifndef GLOBAL_H
#define GLOBAL_H

class Global {
    public:
        int xres, yres;
        int see_wall;
        char keys[65536];
        Global() {
            xres = 640;
            yres = 480;
            memset(keys, 0, 65536);
        }
};

extern Global gl;
#endif

class Wall {
    public:
        float xPos;
        float yPos;
        float xLen;
        float yLen;
        float color[4];

    public:
        Wall (float x, float y, float x_l, float y_l) {
            xPos = x;
            yPos = y;
            xLen = x_l;
            yLen = y_l;
            color[0] = 0.0;
            color[1] = 1.0;
            color[2] = 1.0;
            color[3] = 0.25f;
        }

};

class Door {
public:
    int toRoom;
    int toDoor;
    int locked;
    int isDouble;

public:
    Door() {
        toRoom = -1;
        toDoor = -1;
        locked = -1;
        isDouble = -1;
    }
    Door(int dID, int rID, int l, int d) {
        toRoom = rID;
        toDoor = dID;
        locked = l;
        isDouble = d;
    }
};

class Hole {
public:
    int toHole;
    int blocked;
public:
    Hole() {
        toHole = -1;
        blocked = -1;
    }
    Hole(int hID, int b) {
        toHole = hID;
        blocked = b;
    }
};

class Storage {
public:
    int hasItem;
    int type;
    int collected; // for item render
public:
    Storage(){
        hasItem = -1;
        type = -1;
        collected = -1;
    }
    Storage(int h, int t){
        hasItem = h;
        type = t;
        collected = -1;
    }     
};

// redefine Door to Eventspace
// - This way we can make one button as interact to go to new rooms
// - As well as make it where the player can pick up items and hide

class Eventspace : public Door, public Hole, public Storage{
public:
    int id;
    int etype;
    float xPos;
    float yPos;
    float color[4];
    int facing = -1;
    
    // For door
    Door door;
    
    // For tunnels
    Hole hole;

    // For hiding
    //--
    // For item
    Storage stor;
public:
    // For door
    Eventspace(int i, int t, float x, float y, Door dr, int f){
        id = i;
        etype = t;
        xPos = x;
        yPos = y;
        facing = f;
        door = dr;
        color[0] = 0.647059f;
        color[1] = color[2] = 0.164706f;
        color[3] = 0.25f;
    }

    // For Hiding
    Eventspace(int i, int t, float x, float y) {
        id = i;
        etype = t;
        xPos = x;
        yPos = y;
        color[0] = 0.8f;
        color[1] = 0.498039f;
        color[2] = 0.196078f;
        color[3] = 0.25f;
    }

    // For Item
    Eventspace(int i, int t, float x, float y, Storage s) {
        id = i;
        etype = t;
        xPos = x;
        yPos = y;
        stor = s;
        color[0] = 1.0f;
        color[1] = 0.5f;
        color[2] = 0.0f;
        color[3] = 0.25f;
    }

    // For tunnels in room
    Eventspace(int i, int t, float x, float y, Hole h){
        id = i;
        etype = t;
        xPos = x;
        yPos = y;
        hole = h;
        color[0] = 0.164706f;
        color[1] = color[2] = 0.647059f;
        color[3] = 0.25f;
    }
    
};

class Room {
    public:
        int id;
        std::vector<Wall> walls;
        std::vector<Eventspace> ev;
        //std::vector<Door> doors;
        std::string imagefile;
        std::string imagefile_g;
        //std::vector<Zombie> zombies;

    public:
        Room() {}

        Room(int i, std::vector<Wall> w, std::vector<Eventspace> e, std::string f, std::string g) {
            id = i;
            //while (i < size)
            //{
            walls = w;
            ev = e;
            //doors = d;
            imagefile = f;
            imagefile_g = g;
            //zombies = z;
            //}

        }

        ~Room() {}
};
class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		name[slen-4] = '\0';
		char ppmname[80];
		sprintf(ppmname,"%s.ppm", name);
		char ts[100];
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n", ppmname);
			exit(0);
		}
		unlink(ppmname);
	}
};

class SpriteTexture {
public:
    Image *spriteImage;
    GLuint spriteTexture;

    float xc[2];
    float yc[2];
};

class Sprite {
    public:
        int xres, yres;
        SpriteTexture spTex;
        int spriteFrame;
        double delay;
        int wall;
        Sprite() {}
        // For character sprites (Player or zombies)
        Sprite(int x, int y, int w) {
            xres = x;
            yres = y;
            wall = w;
            delay = 0.1;
            spriteFrame = 0;
        }

        // For item sprites
        Sprite(int x, int y) {
            xres = x;
            yres = y;
        }
};

class Player {
    public:
        Vec pos;
        Vec dir;
        Vec vel;
        Vec acc;
        float angle;
        float color[3];
        float colorAlt[3];
        // Determining facing up or down
        int pFlip;
        int shown;
        int can_move;

        int up_walk, down_walk, left_walk, right_walk;

        Sprite sp;
        int positionState;
        int animationState;
        int condition;
    public:
        Player() {
            pos[0] = (Flt)(gl.xres/2);
            pos[1] = (Flt)(gl.yres/2);
            pos[2] = 0.0f;
            VecZero(dir);
            VecZero(vel);
            VecZero(acc);
            angle = 0.0;
            color[0] = color[1] = color[2] = 1.0;
            colorAlt[0] = colorAlt[1] = colorAlt[2] = 0.658824;
            pFlip = 0;

            shown = 1;
            can_move = 1;
            up_walk = down_walk = left_walk = right_walk = 0;

            sp.xres = 192;
            sp.yres = 64;
            sp.spriteFrame = 0;
            animationState = 0;    // Which frame
            positionState = 0;     // Stance state
            condition = 0;         // Health condition
        }
};

class Zombie {
    public: //Access specifier
        int id; // Identification of the zombie
        int room; // Which room it is in
        int xPos, yPos;
        Vec pos;
        float angle;
        int alive;
        float color[3];
        float w,h; //used to check if item/player hits zombie

        float moveDistance; // Distance moved so far
        float maxDistance; // Maximum distance before turning
        int direction; // 0: up, 1: right, 2: down, 3: left

        int count; // Countdown for zombies
        bool waiting; // See if zombie is waiting
        bool following; // Chek to see if the zombie was following
        //bool sus;

    public:
        //Zombie Constructor
        Zombie(int idZ, float x, float y, float a, int l, int r)
            : moveDistance(0.0f), maxDistance(100.0f), direction(1),
              count(0), waiting(1), following(false) { //member initializer list
                id = idZ;
                pos[0] = x;
                pos[1] = y;
                angle = a;
                alive = l;
                room = r;
                color[0] = 0.0f;
                color[1] = 1.0f;
                color[2] = 0.0f;
                w = 30.0f;
                h = 50.0f;
            }
        ~Zombie() {}
};

class Health {
    public: 
        float pos[2];
        float basepos[2];
        float backpos[2];
        float basew, baseh, backw, backh, w, h;
        Health() {
            float winx = 680.0f;
            float winy = 480.0f;

            basew = 20.0f;
            baseh = 185.0f;

            backw = 15.0f;
            backh = 180.0f;

            w = 15.0f;
            h = 180.0f;

            basepos[0] = winx / 26;
            basepos[1] = (winy / 3) + 80;

            backpos[0] = winx / 26;
            backpos[1] = (winy / 3) + 80;

            pos[0] = winx / 26;
            pos[1] = (winy / 3) + 80;
        }
};

class Pause {
    public:
        float pos[2];
        int w, h;
        int xres, yres;
        Pause() {
            xres = 680;
            yres = 480;
            w = xres;
            h = yres;
            pos[0] = 0;
            pos[1] = 0;
    }
};

class Inventory {
    public:
        float pos[2];
        int w,h;
        int xres,yres;
        Inventory(){
            xres = 640;
            yres = 480;
            w = 100;
            h = 20;
            pos[0] = 0;
            pos[1] = yres / 18;
        }
};

class Axe {
    public:
        int damage;
        float pos[2];
        float w,h;
        bool collected;
        Axe() {
            pos[0] = 370.0f;
            pos[1] = 240.0f;
            collected = false; //for item to render in slots
            damage = 10;
            w = 40.0f;
            h = 10.0f;
        }
        Axe(float posX, float posY) {
            pos[0] = posX;
            pos[1] = posY;
            collected = false; //for item to render in slots
            damage = 10;
            w = 40.0f;
            h = 10.0f;
        }

};

class Healthpack {
    public:
        int heal;
        float pos[2];
        float w,h;
        bool collected;
        int available;
        Healthpack() {
            pos[0] = 70.0f;
            pos[1] = 360.0f;
            w = 5;
            h = 5;
            collected = false;
            heal = 0;
            available = 0;
        }
       Healthpack(float posX, float posY) {
            pos[0] = posX;
            pos[1] = posY;
            w = 5;
            h = 5;
            collected = false;
            heal = 0;
            available = 0;
       }
};

class Key {
    public:
        float pos[2];
        float w,h;
        bool collected;
        int available;
        Key() {
            pos[0] = 70.0f;
            pos[1] = 360.0f;
            w = 5;
            h = 5;
            collected = false;
            available = 0;
        }
       Key(float posX, float posY) {
            pos[0] = posX;
            pos[1] = posY;
            w = 5;
            h = 5;
            collected = false;
            available = 0;
       }
};

