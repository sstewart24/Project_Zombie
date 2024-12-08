//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.

#include "header.h"
Global gl;

//using namespace std;
extern Room startRooms(int);
extern Room swapRoom(int, Room);
extern int *checkEventSpace(Room, float*);

extern int checkWall(float*, Room);
extern float movePlayerToRoom(int);
extern void renderEvent(Eventspace);
extern void renderWall(Wall);
extern void renderZombie(Room, Player); // Renders Zombies
extern void renderHealth(Health, float);
extern void renderInventory();
extern bool pCollision(Player, int);
extern float increaseHealth(float);
extern float damageHealth(float);
extern void backGl();
extern void roomRender(int, int, int, int);
extern void renderLight(int, int);
extern void renderItem(Eventspace);
extern int storageInteract(int, Room);
extern float holeInteract(int, Room, int);
extern void init_Player_Images(Sprite&);
extern void spritePlayerRender(Sprite, float, float, int);
extern void init_Item_Images();
extern void init_World();
extern void init_zomb_Sprites();
extern void clear_run();
extern void renderPause(Pause);
extern Zombie getZombies(int);
extern void init_zombie();
extern void clear_zombie();
extern void renderPlayerItem(float,float);
extern bool zombieAxeCollision(int,int,int);
extern void resetItems();
int roomID = 6;
int see_wall;
int see_darkness;
float pHealth = 180.0f;
int item = 1;
Timers timer;

class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};

class Asteroid {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
public:
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};


class Game {
public:
	Player player;
	Room room;
    Pause p;
    //Zombie zombie;
    //Inventory ibox[IBOX];
    //Inventory iboxbg;
	Health hbox;
    Healthpack hPack;
    Axe axe;

	Asteroid *ahead;
	Bullet *barr;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
	double animationDelay;
	int player_direction;
	int goryOn;
    int pause;
public:
	Game() {
		ahead = NULL;
		barr = new Bullet[MAX_BULLETS];
		nasteroids = 0;
		nbullets = 0;
		mouseThrustOn = false;
        
		animationDelay = 0.15;
		player_direction = 0;
		goryOn = 0;
        pause = 0;
		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
		delete [] barr;
	}
} g;

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl.xres, gl.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen = 0;
		gl.xres = w;
		gl.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			gl.xres = getWinAttr.width;
			gl.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Project_Zombie");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl.xres || xce.height != gl.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		gl.xres = w;
		gl.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(gl.xres, gl.yres);
// ---> for fullscreen x11(0, 0);

//function prototypes
void init_opengl(void);
void roomInit(int);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	see_wall = 1; // Shows the collision boxes of the walls
	init_zomb_Sprites();
	init_Player_Images(g.player.sp);
	init_Item_Images();
	init_World();
	init_zombie();
	roomInit(roomID);

	logOpen();
	init_opengl();
	srand(time(NULL));
	//clock_gettime(CLOCK_REALTIME, &timePause);
	//clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		//clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//timeSpan = timeDiff(&timeStart, &timeCurrent);
		//timeCopy(&timeStart, &timeCurrent);
		//physicsCountdown += timeSpan;
		//while (physicsCountdown >= physicsRate) {
		physics();
			//physicsCountdown -= physicsRate;
		//}
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void roomInit(int rID)
{
	int id = rID;
	g.room = startRooms(id);
}

void init_opengl(void)
{
	backGl();
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	//static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	//keys[XK_Up] = 0;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		}
}

void playerInteract();

int check_keys(XEvent *e)
{
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress) {
		//not a keyboard event
		return 0;
	}
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key] = 0;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift = 0;
			return 0;
		}
		if (key == XK_w || key == XK_Up) {
			g.player.up_walk = 0;
		}
		if (key == XK_s || key == XK_Down) {
			g.player.down_walk = 0;
		}
		if (key == XK_a || key == XK_Left) {
			g.player.left_walk = 0;
		}
		if (key == XK_d || key == XK_Right) {
			g.player.right_walk = 0;
		}
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift = 1;
			return 0;
		}
		if (key == XK_w || key == XK_Up) {
			g.player.up_walk = 1;
		}
		if (key == XK_s || key == XK_Down) {
			g.player.down_walk = 1;
		}
		if (key == XK_a || key == XK_Left) {
			g.player.left_walk = 1;
		}
		if (key == XK_d || key == XK_Right) {
			g.player.right_walk = 1;
		}
		if (key == XK_f) {
			playerInteract();
		}
		if (key == XK_l) {
			// For me to see where wall colissions will be
			see_wall = !see_wall;
		}
        if (key == XK_h) {
            if (g.hPack.collected) {
                pHealth = increaseHealth(pHealth);
            }
       	}
		if (key == XK_g) {
			g.goryOn = !g.goryOn; 
		}
        if (key == XK_p) {
            g.pause = !g.pause;
        }
        if (key == XK_space) {
		gl.pressed = true;
		gl.stun = false;
            	if (g.axe.collected) {
                	bool zac = zombieAxeCollision(g.player.pos[0],g.player.pos[1],g.room.id);
                	if (zac) {
                    		printf("Player attacking Zombie\n");
				gl.pressed = false;
				gl.stun = true;
                	} else {
                    		printf("Player is too far too attack\n");
				gl.pressed = false;
				gl.stun = false;
                	}
            	} 
	}
        }
	(void)shift;
	
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_l:
			break;
		//case XK_g:
		//	see_darkness = !see_darkness;
		//	break;
        //case XK_h:
        //    break;
        /*case XK_e:
            //will be used to collect items in the future
            //only works to toggle the axe on and off for now.
            g.axe.collected ^= 1; 
            break;*/
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
}

int interact_type = -1;
int interact_index = -1;

void playerInteract()
{
	// Pointer to interaction array from individual file
	int* act_ptr;
	act_ptr = checkEventSpace(g.room, g.player.pos);
	interact_type = act_ptr[0];
	interact_index = act_ptr[1];
			
	if (interact_type == 0) {
		// Case 1 would be for the player to interact with doors
		g.room = swapRoom(interact_index, g.room);
		for (int i=0; i<2; i++) {
			g.player.pos[i] = movePlayerToRoom(i);
			g.player.sp.spriteFrame = 0;
		}
	} else if (interact_type == 1) {
		// Case 2 would be for the player to interact with storage/items
		// With limited items, we can set hotbar to specific items
		//Will take interact index and current room, return item type
		int itemtype = storageInteract(interact_index, g.room);
		if (itemtype == -1) {
			printf("Storage empty\n");
		} else {
			switch(itemtype) {
				case 1:
					printf("Grabbed axe\n");
					g.room.ev[interact_index].stor.hasItem = 0;
					g.room.ev[interact_index].stor.collected = 1;
                    g.axe.collected = true;
					break;
                case 2:
                    printf("Grabbed Health Pack\n");
                    g.room.ev[interact_index].stor.hasItem = 0;
                    g.room.ev[interact_index].stor.collected = 1;
                    g.hPack.collected = true;
                    g.hPack.available += 1;
                    break;
                case 3:
                    printf("Grabbed Key\n");
                    g.room.ev[interact_index].stor.hasItem = 0;
                    g.room.ev[interact_index].stor.collected = 1;
                    break;
            }
		}
		fflush(stdout);
		// Calls second function with item type integer and increases the value of that item
		// Based on item, might do certain things
	} else if (interact_type == 2) {
		// Case 3 would be for the player to hide in something
		// Just need to create a flag that makes the player hidden
		g.player.shown = !g.player.shown;
		g.player.can_move = !g.player.can_move;

		// If time, create animation for each specific situation would make it require
		// the interact space index or just call certain gif
	} else if (interact_type == 3) {
		// Case 4 go through a point where endpoints are in the same room
		float npos;
		for (int i=0; i<2; i++) {
			npos = holeInteract(interact_index, g.room, i);
			if (npos != -1.0) {
				g.player.pos[i] = npos;
				g.player.sp.spriteFrame = 0;
			}
		}
	}
			
	interact_type = -1;
	interact_index = -1;
}

void physics()
{
    float xmove = 0.0, ymove = 0.0;
    float newSpeed = sqrt(0.5);
    float newPos[2] = {0.0, 0.0};

	if((g.player.up_walk || g.player.down_walk || 
						   g.player.left_walk || g.player.right_walk) 
						   && g.player.shown) {
		timer.recordTime(&timer.timeCurrent);
		double timeSpan = timer.timeDiff(&timer.walkTime, &timer.timeCurrent);
		if (timeSpan > g.animationDelay) {
			//advance
			++g.player.sp.spriteFrame;
			if (g.player.sp.spriteFrame >= 8)
				g.player.sp.spriteFrame = 0;
			timer.recordTime(&timer.walkTime);
		} 
	}

	//Flt d0,d1,dist;
	//Update player position
	//g.player.pos[0] += g.player.vel[0];
	//g.player.pos[1] += g.player.vel[1];
	//Check for collision with window edges
	if (g.player.pos[0] < 0.0) {
		g.player.pos[0] += (float)gl.xres;
	}
	else if (g.player.pos[0] > (float)gl.xres) {
		g.player.pos[0] -= (float)gl.xres;
	}
	else if (g.player.pos[1] < 0.0) {
		g.player.pos[1] += (float)gl.yres;
	}
	else if (g.player.pos[1] > (float)gl.yres) {
		g.player.pos[1] -= (float)gl.yres;
	}
	
	//---------------------------------------------------
	//check keys pressed now with either: arrow keys or wasd
	if(g.player.can_move) {
		if (gl.keys[XK_Up] || gl.keys[XK_w]) {
	   		// player movement ~ upwards
        	ymove = 1.0;
        	g.player.angle = 360.0f;
        	g.player.pFlip = 1;
    	}
		if (gl.keys[XK_Down] || gl.keys[XK_s]) {
	    	// player movement ~ downwards
        	ymove = -1.0;
        	g.player.angle = 180.0f;
        	g.player.pFlip = 0;
    	}
		if (gl.keys[XK_Left] || gl.keys[XK_a]) {
	    	// player movement ~ left
        	xmove = -1.0;
        	g.player.angle = 90.0f;
			g.player_direction = 1;
		}
		if (gl.keys[XK_Right] || gl.keys[XK_d]) {
	    	// player movement ~ right
        	xmove = 1.0;
        	g.player.angle = 270.0f;
			g.player_direction = 0;
		}
	}
	
    // Makes sure diagnol movement is not faster than if you were to go
    // horizontal (left/right) OR vertical (up/down)
    // xmove/ymove determine state of direction of movement
    // gameSpeed determines base speed of player
    // newSpeed reduces speed of player for going diagnol
    if (pow(xmove, 2) + pow(ymove, 2) > 1) {
        newPos[0] = g.player.pos[0] + (1.0f * xmove * gameSpeed * newSpeed);
        newPos[1] = g.player.pos[1] + (1.0f * ymove * gameSpeed * newSpeed);
    } else {
        newPos[0] = g.player.pos[0] + (1.0f * xmove * gameSpeed);
        newPos[1] = g.player.pos[1] + (1.0f * ymove * gameSpeed);
    }
    if (!(checkWall(newPos, g.room))) { 
        g.player.pos[0] = newPos[0];
        g.player.pos[1] = newPos[1];
        
    }
    
    if (pCollision(g.player, g.room.id)) {
        pHealth = damageHealth(pHealth);
    }
	
}

void render()
{
	roomRender(gl.xres, gl.yres, g.room.id, g.goryOn);
	
	renderZombie(g.room, g.player);
	
	//-------------------------------------------------------------------------
	//Draw the player

    // Placeholder to test character sprite variations
    // if going up/down probably have character actually looking in that
    // direction
	if(g.player.shown) {
    	if (g.player.pFlip) {
	    	glColor3fv(g.player.colorAlt);
    	} else {
	    	glColor3fv(g.player.color);
    	}
		glPushMatrix();
		glTranslatef(g.player.pos[0], g.player.pos[1], g.player.pos[2]);
		//float angle = atan2(player.dir[1], player.dir[0]);
		glRotatef(g.player.angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		//glVertex2f(-10.0f, -10.0f);
		//glVertex2f(  0.0f, 20.0f);
		//glVertex2f( 10.0f, -10.0f);
		glVertex2f(-12.0f, -10.0f);
		glVertex2f(  0.0f,  20.0f);
		glVertex2f(  0.0f,  -6.0f);
		glVertex2f(  0.0f,  -6.0f);
		glVertex2f(  0.0f,  20.0f);
		glVertex2f( 12.0f, -10.0f);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();

		spritePlayerRender(g.player.sp, g.player.pos[0], g.player.pos[1], g.player_direction);
        renderPlayerItem(g.player.pos[0], g.player.pos[1]);
	}
	if (see_wall) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
		for (int i = 0; i != (int)g.room.walls.size(); i++) {
        	renderWall(g.room.walls[i]);
    	}

		for (int i=0; i != (int)g.room.ev.size(); i++) {
			renderEvent(g.room.ev[i]);
		}
		glDisable(GL_BLEND);
	}

	if (see_darkness) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
		renderLight(gl.xres, gl.yres);
		glDisable(GL_BLEND);
	}

	//-------------------------------------------------------------------------
	//Draw Inventory Box 
    renderInventory();
    //-------------------------------------------------------------------------
    //Draw Items
    //renderItem(g.axe);
    //renderItem(g.player, g.room);
    if (item) {
    glBlendFunc(GL_ONE, GL_ZERO); //makes item opaque
    glEnable(GL_BLEND);
    for (int i=0; i!=(int)g.room.ev.size(); i++)
        renderItem(g.room.ev[i]);
    }
    glDisable(GL_BLEND);
    //-------------------------------------------------------------------------
    //Draw Health Box
    
    if (pHealth == 0) {
        clear_run();
	clear_zombie();
    resetItems();
        g.player.pos[0] = (Flt)(gl.xres/2);
	g.player.pos[1] = (Flt)(gl.yres/2);
        pHealth = 180.0f;
        g.room.id = 6;
        roomInit(g.room.id);
        init_World();
	init_zombie();
        render();
    } else {
        renderHealth(g.hbox, pHealth);
    }

    if (g.pause) {
        renderPause(g.p);
        sleep(1);
    }
}

//originally at line 43
//const int IBOX = 4;
/*class Global {
public:
	int xres, yres;
	char keys[65536];
	Global() {
		xres = 640;
		yres = 480;
		memset(keys, 0, 65536);
	}
} gl;*/

/*class Player {
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
	}
};*/

//originally at line 110
//build 10 asteroids...
		/*
        for (int j=0; j<10; j++) {
			Asteroid *a = new Asteroid;
			a->nverts = 8;
			a->radius = rnd()*80.0 + 40.0;
			Flt r2 = a->radius / 2.0;
			Flt angle = 0.0f;
			Flt inc = (PI * 2.0) / (Flt)a->nverts;
			for (int i=0; i<a->nverts; i++) {
				a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
				a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
				angle += inc;
			}
			a->pos[0] = (Flt)(rand() % gl.xres);
			a->pos[1] = (Flt)(rand() % gl.yres);
			a->pos[2] = 0.0f;
			a->angle = 0.0;
			a->rotate = rnd() * 4.0 - 2.0;
			a->color[0] = 0.8;
			a->color[1] = 0.8;
			a->color[2] = 0.7;
			a->vel[0] = (Flt)(rnd()*2.0-1.0);
			a->vel[1] = (Flt)(rnd()*2.0-1.0);
			//std::cout << "asteroid" << std::endl;
			//add to front of linked list
			a->next = ahead;
			if (ahead != NULL)
				ahead->prev = a;
			ahead = a;
			++nasteroids;
		}
        */

//originally at line 335
//Left button is down
			//a little time between each bullet
			//struct timespec bt;
			//clock_gettime(CLOCK_REALTIME, &bt);
			//double ts = timeDiff(&g.bulletTimer, &bt);
			/**
			if (ts > 0.1) {
				//timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					//timeCopy(&b->time, &bt);
					b->pos[0] = g.player.pos[0];
					b->pos[1] = g.player.pos[1];
        init_World();
					b->vel[0] = g.player.vel[0];
					b->vel[1] = g.player.vel[1];
					//convert player angle to radians
					Flt rad = ((g.player.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
					++g.nbullets;
				}
			}
			*/

//oringinally at line 360
//Mouse moved
		//int xdiff = savex - e->xbutton.x;
		//int ydiff = savey - e->xbutton.y;
		//if (++ct < 10)
			//return;		
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
        /*
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.player.angle += 0.05f * (float)xdiff;
			if (g.player.angle >= 360.0f)
				g.player.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.player.angle += 0.05f * (float)xdiff;
			if (g.player.angle < 0.0f)
				g.player.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert player angle to radians
			Flt rad = ((g.player.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			g.player.vel[0] += xdir * (float)ydiff * 0.01f;
			g.player.vel[1] += ydir * (float)ydiff * 0.01f;
			Flt speed = sqrt(g.player.vel[0]*g.player.vel[0]+
												g.player.vel[1]*g.player.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d(g.player.vel);
				g.player.vel[0] *= speed;
				g.player.vel[1] *= speed;
			}
			g.mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
		}
		x11.set_mouse_position(100,100);
		savex = 100;
		savey = 100;
        */

//originally at line 537
/*
    //Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i = 0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
				sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)gl.xres;
		}
		else if (b->pos[0] > (float)gl.xres) {
			b->pos[0] -= (float)gl.xres;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)gl.yres;
		}
		else if (b->pos[1] > (float)gl.yres) {
			b->pos[1] -= (float)gl.yres;
		}
		++i;
	}
	//
	//Update asteroid positions
	Asteroid *a = g.ahead;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl.xres+200;
		}
		else if (a->pos[0] > (float)gl.xres+100) {
			a->pos[0] -= (float)gl.xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl.yres+200;
		}
		else if (a->pos[1] > (float)gl.yres+100) {
			a->pos[1] -= (float)gl.yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}
    
    //
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	a = g.ahead;
	while (a) {
		//is there a bullet within its radius?
		int i=0;
		while (i < g.nbullets) {
			Bullet *b = &g.barr[i];
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//std::cout << "asteroid hit." << std::endl;
				//this asteroid is hit.
				if (a->radius > MINIMUM_ASTEROID_SIZE) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand()%10+5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						Asteroid *ta = new Asteroid;
						buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						ta->next = g.ahead;
						if (g.ahead != NULL)
							g.ahead->prev = ta;
						g.ahead = ta;
						g.nasteroids++;
					}
				} else {
					a->color[0] = 1.0;
					a->color[1] = 0.1;
					a->color[2] = 0.1;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(&g, a);
					a = savea;
					g.nasteroids--;
				}
				//delete the bullet...
				memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
				g.nbullets--;
				if (a == NULL)
					break;
			}
			i++;
		}
		if (a == NULL)
			break;
		a = a->next;
	}
	*/

//orginally at line 609
    /*
	if (gl.keys[XK_space]) {
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (ts > 0.1) {
			timeCopy(&g.bulletTimer, &bt);
			if (g.nbullets < MAX_BULLETS) {
				//shoot a bullet...
				//Bullet *b = new Bullet;
				Bullet *b = &g.barr[g.nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = g.player.pos[0];
				b->pos[1] = g.player.pos[1];
				b->vel[0] = g.player.vel[0];
				b->vel[1] = g.player.vel[1];
				//convert player angle to radians
				Flt rad = ((g.player.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				g.nbullets++;
			}
		}
	}
	if (g.mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
		//std::cout << "tdif: " << tdif << std::endl;
		if (tdif < -0.3)
			g.mouseThrustOn = false;
	}
	*/
	
//line 672
    /*
	for (int i=0; i != (int)g.room.zombies.size(); i++) {
		renderZombie(g.room.zombies[i]);
	} 
    */

//line 708
    /*
	if (gl.keys[XK_Up] || g.mouseThrustOn) {
		int i;
		//draw thrust
		Flt rad = ((g.player.angle+90.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (i=0; i<16; i++) {
			xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
			ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
			r = rnd()*40.0+40.0;
			xe = -xdir * r + rnd() * 18.0 - 9.0;
			ye = -ydir * r + rnd() * 18.0 - 9.0;
			glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
			glVertex2f(g.player.pos[0]+xs,g.player.pos[1]+ys);
			glVertex2f(g.player.pos[0]+xe,g.player.pos[1]+ye);
		}
		glEnd();
	}
   	 */

	//-------------------------------------------------------------------------
	/*
	//Draw Zombie
	glPushMatrix();
	glTranslatef(g.zombie.pos[0], g.zombie.pos[1], g.zombie.pos[2]);
	//float angle = atan2(player.dir[1], player.dir[0]);
	glRotatef(g.zombie.angle, 0.0f, 0.0f, 1.0f);
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
	*/
	//-------------------------------------------------------------------------
	//Draw the asteroids
	/*
	{
		Asteroid *a = g.ahead;
		while (a) {
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			for (int j=0; j<a->nverts; j++) {
				glVertex2f(a->vert[j][0], a->vert[j][1]);
			}
			glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
		}
	}
	//-------------------------------------------------------------------------
	//Draw the bullets
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	}
	
    */

//line 672
    /*g.iboxbg.pos[0] = g.iboxbg.xres / 2;
    renderInventory(g.iboxbg,IBOX+1, g.iboxbg.pos[0], IBOX);
    for (int i=0; i<IBOX; i++) {
        float iboxX = g.ibox[i].pos[0];
        iboxX = (g.ibox[i].xres / 2.75) + (iboxX / 2) + (i * 60);
        g.ibox[i].h = 20;
        g.ibox[i].w = 20;

        renderInventory(g.ibox[i], i, iboxX, IBOX);
    }*/

