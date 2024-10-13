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
            {Door(0, 590.0f, 50.0f, 50.0f, 50.0f, 1),
             Door(1, 0.0f, 380.0f, 50.0f, 50.0f, 1)}), 
    Room(1, {Wall(0.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 0.0f, 640.0f, 50.0f),
             Wall(590.0f, 0.0f, 50.0f, 480.0f),
             Wall(0.0f, 430.0f, 640.0f, 50.0f),
             Wall(400.0f, 150.0f, 75.0f, 100.0f)},
            {Door(0, 0.0f, 150.0f, 50.0f, 50.0f, 0)})
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

int checkDoor(Room current, float playerPos[2])
{
    int i = 0, s = 0;
    float xDiff = 50.0, yDiff = 50.0, xxDiff = 50.0, yyDiff = 50.0;
    int size = (int)current.doors.size();
    int nextID = -1;
    float pl[2];
    pl[0] = playerPos[0];
    pl[1] = playerPos[1];
    
    while (i < size && !s) {
        xDiff = abs(pl[0]- current.doors[i].xPos);
        xxDiff = abs(current.doors[i].xPos + 50.0f - pl[0]);
        yDiff = abs(pl[1]- current.doors[i].yPos);
        yyDiff = abs(current.doors[i].yPos + 50.0f - pl[1] );

        if ((xDiff < 25 && pl[1] > current.doors[i].yPos && pl[1] < current.doors[i].yPos + 50.0f) ||
            (xxDiff < 25 && pl[1] > current.doors[i].yPos && pl[1] < current.doors[i].yPos + 50.0f) || 
            (yDiff < 25 && pl[0] > current.doors[i].xPos && pl[0] < current.doors[i].xPos + 50.0f) || 
            (yyDiff < 25 && pl[0] > current.doors[i].xPos && pl[0] < current.doors[i].xPos + 50.0f)) {
            roomSave(current);
            nextID = current.doors[i].toRoom;
            s = 1;
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
