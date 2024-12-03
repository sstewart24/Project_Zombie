#include "header.h"
#define DOOR_WIDTH 60.0
#define DOORMAT_WIDTH 64.0
#define ITEM_INTERACT_WIDTH 45.0
#define TUNNEL_WIDTH 45.0
// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.

// World mapping
// Includes walls and other objects
Room defaultWorld[] = { 
    Room(0, {Wall(0.0f, 0.0f, 48.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 64.0f),
                Wall(608.0f, 0.0f, 32.0f, 480.0f),
                Wall(0.0f, 448.0f, 640.0f, 32.0f),
                Wall(0.0f, 240.0f, 224.0f, 32.0f),
                Wall(288.0f, 0.0f, 32.0f, 192.0f),
                Wall(448.0f, 0.0f, 32.0f, 192.0f),
                Wall(288.0f, 272.0f, 32.0f, 80.0f),
                Wall(288.0f, 272.0f, 192.0f, 32.0f),
                Wall(448.0f, 272.0f, 32.0f, 224.0f)},
            {Eventspace(0, 0, 482.0f, 368.0f, Door(0, 2, 0, 0), 2),
                Eventspace(1, 0, 98.0f, 368.0f, Door(0, 1, 0, 0), 2),
                Eventspace(2, 0, 583.0f, 240.0f, Door(0, 6, 0, 0), 3),
                Eventspace(3, 3, 304.0f, 80.0f, Hole(4, 0)),
                Eventspace(4, 3, 272.0f, 80.0f, Hole(3, 0)),
                Eventspace(5, 1, 360.0f, 220.0f, Storage(1, 1)),
                Eventspace(6, 2, 272.0f, 320.0f)}, 
            "./images/lab-entrance.png", "./images/lab-entrance-v2.png"), 
    Room(1, {Wall(0.0f, 0.0f, 160.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 96.0f),
                Wall(480.0f, 0.0f, 160.0f, 480.0f),
                Wall(0.0f, 368.0f, 640.0f, 112.0f)},
            {Eventspace(0, 0, 352.0f, 96.0f, Door(1, 0, 0, 0), 1)}, 
            "./images/Lab-office.png", "./images/Lab-office-v2.png"),
    Room(2, {Wall(0.0f, 0.0f, 64.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 80.0f),
                Wall(0.0f, 416.0f, 640.0f, 64.0f),
                Wall(576.0f, 0.0f, 64.0f, 480.0f),
                Wall(256.0f, 80.0f, 320.0f, 64.0f)},
            {Eventspace(0, 0, 130.0f, 80.0f, Door(0, 0, 0, 0), 1),
                Eventspace(1, 0, 64.0f, 210.0f, Door(0, 3, 0, 0), 4),
                Eventspace(2, 0, 386.0f, 311.0f, Door(0, 4, 0, 0), 1),
                Eventspace(3, 0, 130.0f, 311.0f, Door(0, 5, 0, 0), 1)}, 
            "./images/Lab-Hall.png", "./images/Lab-Hall-v2.png"),
    Room(3, {Wall(0.0f, 0.0f, 256.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 80.0f),
                Wall(384.0f, 0.0f, 256.0f, 480.0f),
                Wall(0.0f, 352.0f, 640.0f, 128.0f)},
            {Eventspace(0, 0, 358.0f, 146.0f, Door(1, 2, 0, 0), 4)}, 
            "./images/Lab-closet.png", "./images/Lab-closet-v2.png"),
    Room(4, {Wall(0.0f, 0.0f, 64.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 80.0f),
                Wall(608.0f, 0.0f, 32.0f, 480.0f),
                Wall(0.0f, 448.0f, 640.0f, 32.0f),
                Wall(320.0f, 224.0f, 32.0f, 256.0f)},
            {Eventspace(0, 0, 130.0f, 80.0f, Door(2, 2, 0, 0), 1)}, 
            "./images/lab-sidelab.png", "./images/lab-sidelab-v2.png"),
    Room(5, {Wall(0.0f, 0.0f, 176.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 80.0f),
                Wall(432.0f, 0.0f, 208.0f, 480.0f),
                Wall(0.0f, 352.0f, 640.0f, 128.0f)},
            {Eventspace(0, 0, 370.0f, 80.0f, Door(3, 2, 0, 0), 1)}, 
            "./images/Lab-restroom.png", "./images/Lab-restroom-v2.png"),
    Room(6, {Wall(0.0f, 0.0f, 64.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 64.0f),
                Wall(608.0f, 0.0f, 32.0f, 480.0f),
                Wall(0.0f, 448.0f, 640.0f, 32.0f),
                Wall(160.0f, 0.0f, 64.0f, 176.0f),
                Wall(384.0f, 0.0f, 64.0f, 176.0f)},
            {Eventspace(0, 0, 64.0f, 180.0f, Door(2, 0, 0, 0), 4),
                Eventspace(1, 0, 512.0f, 352.0f, Door(0, 7, 0, 0), 1)}, 
            "./images/lab-testlab.png", "./images/lab-testlab-v2.png"),
    Room(7, {Wall(0.0f, 0.0f, 288.0f, 480.0f),
                Wall(0.0f, 0.0f, 640.0f, 176.0f),
                Wall(352.0f, 0.0f, 288.0f, 480.0f),
                Wall(0.0f, 320.0f, 640.0f, 160.0f)},
            {Eventspace(0, 0, 288.0f, 176.0f, Door(1, 6, 0, 0), 1)}, 
            "./images/Lab-vault.png", "./images/Lab-vault.png")
};

std::vector<Room> rooms;
int roomAmount = 8;
// Helps have a default world in case the player dies and restarts
void init_World()
{
    int i = 0;
    while (i < roomAmount) {
        rooms.push_back(defaultWorld[i]);
        i++;
    }
}
// Removes the rooms so that the index values stay consistent
void clear_run()
{
    int i = 0;
    while (i < roomAmount) {
        rooms.pop_back();
        i++;
    }
}

float jumpPlayerPos_to[2];

class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
};

Texture tex;
std::vector<Texture> textures;
std::vector<Texture> textures_gory;


void backGl()
{
    int i = 0;
    while (i < roomAmount) {
        const char *stringfile = rooms[i].imagefile.c_str();
        Image img[1] = {stringfile};

        //load the images file into a ppm structure.
	    //
	    tex.backImage = &img[0];
	    //create opengl texture elements
	    glGenTextures(1, &tex.backTexture);
	    int w = tex.backImage->width;
	    int h = tex.backImage->height;
	    glBindTexture(GL_TEXTURE_2D, tex.backTexture);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, tex.backImage->data);
	    tex.xc[0] = 0.0;
	    tex.xc[1] = 1.0;
	    tex.yc[0] = 0.0;
	    tex.yc[1] = 1.0;  
        textures.push_back(tex);
        i++;
    }

    i = 0;
    while (i < roomAmount) {
        const char *stringfile = rooms[i].imagefile_g.c_str();
        Image img[1] = {stringfile};

        //load the images file into a ppm structure.
	    //
	    tex.backImage = &img[0];
	    //create opengl texture elements
	    glGenTextures(1, &tex.backTexture);
	    int w = tex.backImage->width;
	    int h = tex.backImage->height;
	    glBindTexture(GL_TEXTURE_2D, tex.backTexture);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, tex.backImage->data);
	    tex.xc[0] = 0.0;
	    tex.xc[1] = 1.0;
	    tex.yc[0] = 0.0;
	    tex.yc[1] = 1.0;  
        textures_gory.push_back(tex);
        i++;
    }   
}

SpriteTexture spr;

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}

	return newdata;
}
//SpriteTexture spTex;
void spriteGl(std::string imagefile)//Sprite &sp, std::string imagefile)
{
    const char *stringfile = imagefile.c_str();
    Image img[1] = {stringfile};

    spr.spriteImage = &img[0];
    //load the images file into a ppm structure.
	//
	//create opengl texture elements
	glGenTextures(1, &spr.spriteTexture);
    int w = spr.spriteImage->width;
    int h = spr.spriteImage->height;
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, spr.spriteTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *spriteData = buildAlphaData(&img[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, spriteData);

    spr.xc[0] = 0.0;
	spr.xc[1] = 1.0;
	spr.yc[0] = 0.0;
	spr.yc[1] = 1.0;
}

void spriteInit(Sprite &sp, std::string image_file)
{
    spriteGl(image_file);
    sp.spTex = spr;
}

class Doormat 
{
public:
    float x;
    float y;
    float xx;
    float yy;

public:
    Doormat(int facing, float door_x, float door_y)
    {
        // Simplify facing to be to different directions: up-down & left-right
        // . - Just need to tweak positions of where door interactions are
        // .
        // .
        // North - South, facing = 1 (North) or 2 (South)
        // West - East, facing = 3 (West) or 4 (East)
        if(facing > 0) {
            if (facing < 3) {
                x = door_x;
                xx = door_x + DOOR_WIDTH;
                switch (facing) {
                    case 1:
                        y = door_y;
                        yy = door_y + DOORMAT_WIDTH;
                        break;
                    case 2:
                        y = door_y - DOORMAT_WIDTH;
                        yy = door_y;
                        break;
                }
            } else {
                y = door_y;
                yy = door_y + DOOR_WIDTH;
                switch (facing) {
                    case 3:
                        x = door_x - DOORMAT_WIDTH;
                        xx = door_x;
                        break;
                    case 4:
                        x = door_x;
                        xx = door_x + DOORMAT_WIDTH;
                        break;
                }
            }
        } else {
            y = door_y;
            x = door_x;
            xx = door_x + TUNNEL_WIDTH;
            yy = door_y + TUNNEL_WIDTH;
        }
    }
};
// Initializes first room to make
Room startRooms(int roomID) {
    Room r = rooms[roomID];
    return r;
}

// Saves data of current room back into Room array
void roomSave(Room cur)
{
    rooms[cur.id] = cur;
}
// Sets the position of the player going into the next room

void findDoor(int nextDoor, int nextRoom) 
{
    Room next_room = rooms[nextRoom];
    Doormat next_door = Doormat(next_room.ev[nextDoor].facing, next_room.ev[nextDoor].xPos, next_room.ev[nextDoor].yPos);
    jumpPlayerPos_to[0] = next_door.x + (next_door.xx - next_door.x)/2;
    jumpPlayerPos_to[1] = next_door.y + (next_door.yy - next_door.y)/2;
}

// Will from one room to the next based on interacting with a door

Room swapRoom(int doorID, Room current)
{
    int nextDoorID = current.ev[doorID].door.toDoor;
    int nextRoomID = current.ev[doorID].door.toRoom;

    Room next;
    roomSave(current);
    findDoor(nextDoorID, nextRoomID);
    next = rooms[nextRoomID];
    return next;
}

int storageInteract(int storageID, Room current)
{
    int item = -1;
    if (current.ev[storageID].stor.hasItem == 1) {
        item = current.ev[storageID].stor.type;
    }
    return item;
}

void findHole(int nextHole, int rid) {
    Doormat next_hole = Doormat(rooms[rid].ev[nextHole].facing, rooms[rid].ev[nextHole].xPos, rooms[rid].ev[nextHole].yPos);
    jumpPlayerPos_to[0] = next_hole.x + (next_hole.xx - next_hole.x)/2;
    jumpPlayerPos_to[1] = next_hole.y + (next_hole.yy - next_hole.y)/2;
}

float holeInteract(int holeID, Room current, int i)
{
    int nextPos = -1.0;
    if (!current.ev[holeID].hole.blocked) {
        findHole(current.ev[holeID].hole.toHole, current.id);
        nextPos = jumpPlayerPos_to[i];
    }

    return nextPos;
}


int interaction[2];
//checks to see if the player is with an area of interaction
//returns what type of interaction it is: (go to room, get item)
int* checkEventSpace(Room current, float pl_pos[2]) {
    int i = 0, act = 0;
    int size = (int)current.ev.size();
    // [0] = type of interaction,  [1] = index of interaction space
    // set to -1 in the case where player is not within
    // interaction space
    interaction[0] = -1;
    interaction[1] = -1;

    while (i < size && !act) {
        Doormat area = Doormat(current.ev[i].facing, current.ev[i].xPos, current.ev[i].yPos);//, current.ev[i].width, current.ev[i].height);

        if (pl_pos[0] > area.x && pl_pos[0] < area.xx && 
                pl_pos[1] > area.y && pl_pos[1] < area.yy) {
            act = 1;
            interaction[0] = current.ev[i].etype; 
            interaction[1] = i;
        }
        i++;
    }

    return interaction;
}


// Will check if the player is colliding with a wall to block any movement
int checkWall(float newPos[2], Room room) {
    std::vector<Wall> w = room.walls;
    int blocked = 0;
    int i = 0;
    while (!blocked && i < (int)w.size()) {
        if (newPos[0] + 16 > w[i].xPos && newPos[0] - 16 < w[i].xPos + w[i].xLen && 
                newPos[1] > w[i].yPos && newPos[1] - 32 < w[i].yPos + w[i].yLen) {
            blocked = 1;
        }
        i++;
    }

    return blocked;
}

float movePlayerToRoom(int index) {
    return jumpPlayerPos_to[index];
}

void roomRender(int xres, int yres, int i, int gory) 
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
    if (gory)
        glBindTexture(GL_TEXTURE_2D, textures_gory[i].backTexture);
    else
	    glBindTexture(GL_TEXTURE_2D, textures[i].backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(tex.xc[0], tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(tex.xc[0], tex.yc[0]); glVertex2i(0, yres);
		glTexCoord2f(tex.xc[1], tex.yc[0]); glVertex2i(xres, yres);
		glTexCoord2f(tex.xc[1], tex.yc[1]); glVertex2i(xres, 0);
	glEnd();
    glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderWall(Wall wall) {
    glPushMatrix();
        glColor4fv(wall.color);
        glBegin(GL_TRIANGLES);

        glVertex2f(wall.xPos, wall.yPos);
        glVertex2f(wall.xPos + wall.xLen, wall.yPos);
        glVertex2f(wall.xPos + wall.xLen, wall.yPos + wall.yLen);
        glVertex2f(wall.xPos, wall.yPos);
        glVertex2f(wall.xPos, wall.yPos + wall.yLen);
        glVertex2f(wall.xPos + wall.xLen, wall.yPos + wall.yLen);
        
        glEnd();
    glPopMatrix();
}

void renderEvent(Eventspace e)
{
    Doormat door_mat = Doormat(e.facing, e.xPos, e.yPos);
    glPushMatrix();
        glColor4fv(e.color);
        glBegin(GL_TRIANGLES);

        glVertex2f(door_mat.x, door_mat.y);
        glVertex2f(door_mat.xx, door_mat.y);
        glVertex2f(door_mat.xx, door_mat.yy);
        glVertex2f(door_mat.x, door_mat.y);
        glVertex2f(door_mat.x, door_mat.yy);
        glVertex2f(door_mat.xx, door_mat.yy);
        glEnd();
    glPopMatrix();
}

void renderLight(int xres, int yres)
{
    // Basic sense of darkness of the game, should modify for light areas
    // and light around the player
    glPushMatrix();
        glColor4f(0.0f, 0.0f, 0.0f, 0.92f);

        glBegin(GL_TRIANGLES);
        glVertex2f(0, 0);
        glVertex2f(xres, 0);
        glVertex2f(xres, yres);
        glVertex2f(0, 0);
        glVertex2f(0, yres);
        glVertex2f(xres, yres);
        glEnd();
    glPopMatrix();
}
