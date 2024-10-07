#include "header.h"


// Builds the room to be drawn for any objects in the room
// This determines the rooms walls.


// Good for objects that won't change, this isn't having info actually stored
Room r0;
    r0.id = 0;
    r0.walls = {Wall(50.0f, 100.0f, 50.0f, 50.0f),
                Wall(75.0f, 200.0f, 100.0f, 50.0f),
                Wall(200.0f, 200.0f, 30.0f, 50.0f),
                Wall(400.0f, 50.0f, 200.0f, 100.0f)}; 
Room r1;
    r1.id = 0,
    r1.walls = {Wall(100.0f, 100.0f, 50.0f, 50.0f),
                Wall(175.0f, 200.0f, 100.0f, 50.0f),
                Wall(300.0f, 100.0f, 30.0f, 50.0f),
                Wall(400.0f, 450.0f, 75.0f, 100.0f)};
                
void roomSave(Room prev)
{
    
}

Room swapRoom(int roomID, Room prev) {
    Room next;
    switch (roomID) {
        case 0:
            
            next = r0;
            break;
            case 1:
            next = r1;

            break;
    }
    return next;
}

Room roomStart() 
{
    Room room = swapRoom(0);
    return room;
}
