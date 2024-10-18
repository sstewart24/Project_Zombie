#include "header.h"
#define DOOR_WIDTH 50.0
#define DOORMAT_WIDTH 25.0
// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.

// World mapping
// Includes walls and other objects
Room rooms[] = { 
    Room(0, {Wall(0.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 50.0f),
             Wall(590.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 430.0f, 640.0f, 50.0f),
             Wall(200.0f, 100.0f, 440.0f, 50.0f)},
            {Door(0, 590.0f, 50.0f, 0, 1, 3),
             Door(1, 50.0f, 380.0f, 0, 1, 4)}), 
    Room(1, {Wall(0.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 50.0f),
             Wall(590.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 430.0f, 640.0f, 50.0f),
             Wall(400.0f, 150.0f, 75.0f, 100.0f)},
            {Door(0, 50.0f, 150.0f, 1, 0, 4)})
};

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

void renderWall(Wall wall) {
    glPushMatrix();
        glColor3fv(wall.color);
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
    glColor3fv(door.color);
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