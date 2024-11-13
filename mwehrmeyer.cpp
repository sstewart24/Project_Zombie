#include "header.h"
#define DOOR_WIDTH 60.0
#define DOORMAT_WIDTH 25.0
// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.

// World mapping
// Includes walls and other objects
Room rooms[] = { 
    Room(0, {Wall(0.0f, 0.0f, 32.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 32.0f),
             Wall(608.0f, 0.0f, 32.0f, 480.0f),
             Wall(0.0f, 448.0f, 640.0f, 32.0f),
             Wall(0.0f, 224.0f, 224.0f, 32.0f),
             Wall(288.0f, 0.0f, 32.0f, 144.0f),
             Wall(448.0f, 0.0f, 32.0f, 144.0f)},
            {Door(0, 482.0f, 368.0f, 0, 1, 2),
             Door(1, 98.0f, 368.0f, 0, 1, 2)}, "room1.png"), 
    Room(1, {Wall(0.0f, 0.0f, 160.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 96.0f),
             Wall(480.0f, 0.0f, 160.0f, 480.0f),
             Wall(0.0f, 368.0f, 640.0f, 112.0f)},
            {Door(0, 352.0f, 96.0f, 1, 0, 1)}, "officeroom.png") // shorten the door
};

class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
};

Texture tex;
std::vector<Texture> textures;
int roomAmount = 2;
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
}

float jumpPlayerPos_to[2];

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
        // North - South, facing = 1 (North) or 2 (South)
        // West - East, facing = 3 (West) or 4 (East)
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

// Will from one room to the next based on interacting with a door
Room swapRoom(int roomID, Room current) {
    //int checkDoor(current, player_pos);
    Room next;
    roomSave(current);
    next = rooms[roomID];
    return next;
}

int checkDoor(Room current, float playerPos[2])
{
    int i = 0, s = 0;
    int size = (int)current.doors.size();
    int nextID = -1;
    
    while (i < size && !s) {
        Doormat door_mat = Doormat(current.doors[i].facing, current.doors[i].xPos, current.doors[i].yPos);

        if (playerPos[0] > door_mat.x && playerPos[0] < door_mat.xx && 
                playerPos[1] > door_mat.y && playerPos[1] < door_mat.yy) {
            roomSave(current);
            nextID = current.doors[i].toRoom;
            s = 1;

            int d = current.doors[i].toDoor;
            Doormat next_door = Doormat(rooms[nextID].doors[d].facing, rooms[nextID].doors[d].xPos, rooms[nextID].doors[d].yPos);
            jumpPlayerPos_to[0] = next_door.x + (next_door.xx - next_door.x)/2;
            jumpPlayerPos_to[1] = next_door.y + (next_door.yy - next_door.y)/2;

        }
        i++;
    }

    return nextID;
}

// Will check if the player is colliding with a wall to block any movement
int checkWall(float newPos[2], Room room) {
    std::vector<Wall> w = room.walls;
    int blocked = 0;
    int i = 0;
    while (!blocked && i < (int)w.size()) {
        if (newPos[0] > w[i].xPos && newPos[0] < w[i].xPos + w[i].xLen && 
            newPos[1] > w[i].yPos && newPos[1] < w[i].yPos + w[i].yLen) {
            blocked = 1;
        }
        i++;
    }

    return blocked;
}
/*
void renderDoor(Door door) {
    glPushMatrix();
        glColor3fv(door.color);
        glBegin(GL_TRIANGLES);

        glVertex2f(door.xPos, door.yPos);
        glVertex2f(door.xPos + door.xLen, door.yPos);
        glVertex2f(door.xPos + door.xLen, door.yPos + door.yLen);
        glVertex2f(door.xPos, door.yPos);
        glVertex2f(door.xPos, door.yPos + door.yLen);
        glVertex2f(door.xPos + door.xLen, door.yPos + door.yLen);
        glEnd();
        glPopMatrix();
}
*/
float movePlayerToRoom(int index) {
    return jumpPlayerPos_to[index];
}

void roomRender(int xres, int yres, int i) 
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
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

void renderDoorEvent(Door door)
{
    Doormat door_mat = Doormat(door.facing, door.xPos, door.yPos);
    glPushMatrix();
        glColor4fv(door.color);
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