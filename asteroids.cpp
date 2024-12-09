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
extern void renderInventory(Healthpack,Key,Axe);
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
extern void updateInventory(int);
int roomID = 0;
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
    Key key;

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
            if (g.hPack.collected && g.hPack.available > 0) {
                pHealth = increaseHealth(pHealth);
                g.hPack.available -= 1;
                printf("player health increased\n");
            }       	}
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
                    g.hPack.available += 1;
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
                    g.key.collected = true;
                    g.key.available += 1;
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
    //updateInventory(g.hPack.collected);
    //renderinventory();
    renderInventory(g.hPack, g.key, g.axe);
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
        //g.room.id = 6;
		init_World();
        roomInit(roomID);
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

