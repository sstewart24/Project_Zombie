#include "header.h"

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
            {Door(590.0f, 50.0f, 50.0f, 50.0f, 1),
             Door(0.0f, 380.0f, 50.0f, 50.0f, 1)}), 
    Room(1, {Wall(0.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 50.0f),
             Wall(590.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 430.0f, 640.0f, 50.0f),
             Wall(400.0f, 150.0f, 75.0f, 100.0f)},
             {Door(50.0f, 150.0f, 50.0f, 50.0f, 0)})
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
    Room next;
    roomSave(current);
    next = rooms[roomID];
    return next;
}
/*
Room checkDoor(int id, Room current, float playerPos[2])
{
    float xDiff , yDiff;
    for (int i=0; i!=(int)current.doors.size(); i++) {
        xDiff = abs(playerPos[0]- current.doors[i].xPos);
        yDiff = abs(playerPos[1]- current.doors[i].yPos);

        if (xDiff < 25 && yDiff < 25) {
            int nextRoomID = swapRoom(current.doors[i].toRoom, current);
            return rooms[nextRoomID];
        }
    }
}
*/

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
