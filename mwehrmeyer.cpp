#include "header.h"

// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.

// World mapping
// Includes walls and other objects
Room rooms[] = { 
    Room(0, {Wall(50.0f, 100.0f, 50.0f, 50.0f),
             Wall(75.0f, 200.0f, 100.0f, 50.0f),
             Wall(200.0f, 200.0f, 30.0f, 50.0f),
             Wall(400.0f, 50.0f, 200.0f, 100.0f)},
             {Door(50.0f, 150.0f, 50.0f, 50.0f, 1)}), 
    Room(1, {Wall(100.0f, 100.0f, 50.0f, 50.0f),
             Wall(175.0f, 200.0f, 100.0f, 50.0f),
             Wall(300.0f, 100.0f, 30.0f, 50.0f),
             Wall(400.0f, 450.0f, 75.0f, 100.0f),
             Wall(400.0f, 150.0f, 75.0f, 100.0f)},
             {Door(50.0f, 150.0f, 50.0f, 50.0f, 1)})
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
