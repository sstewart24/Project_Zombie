#include "header.h"


// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.


// Good for objects that won't change, this isn't having info actually stored
Room rooms[] = { 
    Room(0, {Wall(50.0f, 100.0f, 50.0f, 50.0f),
             Wall(75.0f, 200.0f, 100.0f, 50.0f),
             Wall(200.0f, 200.0f, 30.0f, 50.0f),
             Wall(400.0f, 50.0f, 200.0f, 100.0f)}), 
    Room(1, {Wall(100.0f, 100.0f, 50.0f, 50.0f),
             Wall(175.0f, 200.0f, 100.0f, 50.0f),
             Wall(300.0f, 100.0f, 30.0f, 50.0f),
             Wall(400.0f, 450.0f, 75.0f, 100.0f)})
};

Room startRooms(int roomID) {
    Room r = rooms[roomID];
    //Room r = Room(rooms[roomID].id, rooms[roomID].walls);
    return r;
}

void roomSave(Room cur)
{
    rooms[cur.id] = cur;
}

Room swapRoom(int roomID, Room current) {
    Room next;
    roomSave(current);
    next = rooms[roomID];
    return next;
}

