/* Lyanne Rafanan's Source File
 * last updated: 07 DEC 24
 * What's in here?
 *  - Render and initialize values for Inventory Box
 *  - Render Collectable Items:
 *      - axe(1): stun/kill zombies
 *      - healthpack(multiple around the map): incr player health
 *      - key(1) : used to win the game maybe lol
 *  - Zombie axe Collision check
 *  - Item reset function when player dies
 *  - Render axe next to player if player collects the item
 *
 * */
#include "header.h"
// Funtion Prototypes:
extern void spriteInit(Sprite&, std::string);
void init_Item_Images();
void spriteItemRender(Sprite sp, float xPos, float yPos);
void init_inventory();
void renderInventory(Eventspace);
void renderItem(Eventspace);
void renderPlayerItem(float playerX, float playerY);
bool zombieAxeCollision(int playerX,int playerY, int id);
void resetItems();

//Globals
Key key (240.0f, 200.0f);
Healthpack healthpack (70.0f, 350.0f);
Axe axe (370.0f, 240.0f);
const int MAXINVENTORY = 4;
Inventory box[MAXINVENTORY];
Inventory bg;

Sprite healthpack_img = Sprite(40, 40);
std::string hp_imagefile = "./images/Health-pack.png"; 
Sprite axe_img = Sprite(40, 40);
std::string axeInv_imagefile = "./images/AxeSprite-Inventory.png";
Sprite key_img = Sprite(40, 40);
std::string keyInv_imagefile = "./images/Sprite-key.png";

// Initializing Images for render
void init_Item_Images() {
    spriteInit(healthpack_img, hp_imagefile);
    spriteInit(axe_img, axeInv_imagefile);
    spriteInit(key_img, keyInv_imagefile);
}

// Render the item into an inventory slot, called in renderInventory()
void spriteItemRender(Sprite sp, float xPos, float yPos) {
    float zPos = 0.0f;
    float cx = sp.xres / 2;
    float cy = sp.yres / 2;
    float tx = 0.0;
    float ty = 0.0;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    glTranslatef(xPos, yPos, zPos);
        glBegin(GL_QUADS);
        glTexCoord2f(tx, ty+1.0);     glVertex2i(-cx, -cy);
        glTexCoord2f(tx, ty);         glVertex2i(-cx, cy);
        glTexCoord2f(tx+1.0, ty);     glVertex2i(cx, cy);
        glTexCoord2f(tx+1.0, ty+1.0); glVertex2i(cx, -cy);
   glEnd();
   glPopMatrix();
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_ALPHA_TEST);
}

// Initializing Positions for the Inventory Box
void init_inventory() {
    bg.pos[0] = bg.xres / 2;
    bg.w = 100;
    bg.h = 20;

    for (int i=0; i<MAXINVENTORY; i++) {
        box[i].pos[0] = (box[i].xres / 2.75) + (i * 120) / 2;
        box[i].h = 20;
        box[i].w = 20;
    }
}

// Rendering the inventory box and its slots for collectable items
void renderInventory() {
    init_inventory();

    //background box
    glPushMatrix();
    glColor3ub(74, 78, 105);
    glTranslatef(bg.pos[0], bg.pos[1], 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-bg.w, -bg.h);
        glVertex2f(-bg.w,  bg.h);
        glVertex2f( bg.w,  bg.h);
        glVertex2f( bg.w, -bg.h);
    glEnd();
    glPopMatrix();

    //inventory slots
    for (int i=0; i<MAXINVENTORY; i++) {
    glPushMatrix();
        glColor3ub(34, 34, 59);
        glTranslatef(box[i].pos[0], box[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box[i].w, -box[i].h);
            glVertex2f(-box[i].w,  box[i].h);
            glVertex2f( box[i].w,  box[i].h);
            glVertex2f( box[i].w, -box[i].h);
       glEnd();
       glPopMatrix();

    // Adding Items into inventory slots
    if (i==0 && axe.collected == 1)
        spriteItemRender(axe_img, box[i].pos[0], box[i].pos[1]);
    if (i==1 && healthpack.collected == 1)
        spriteItemRender(healthpack_img, box[i].pos[0], box[i].pos[1]);
    if (i==2 && key.collected == 1)
        spriteItemRender(key_img, box[i].pos[0], box[i].pos[1]);
    }
}

// Rendering Items to Collect on the Map
void renderItem(Eventspace e) {
    //Axe - one inside of main lobby
    if (e.stor.hasItem != -1 &&
        e.stor.collected != 1 &&
        e.stor.type == 1) {
        spriteItemRender(axe_img, e.stor.item_xPos, 
                                    e.stor.item_yPos);
    }
    else if (e.stor.collected == 1 && e.stor.type == 1){
        axe.collected = 1; //used to update inventory slot render
    }

    //Health packs - Multiple around different rooms
    if (e.stor.hasItem != -1 &&
        e.stor.collected != 1 &&
        e.stor.type == 2) {
        spriteItemRender(healthpack_img, e.stor.item_xPos, 
                                            e.stor.item_yPos);
    }
    else if (e.stor.collected == 1 && e.stor.type == 2){
        healthpack.collected = 1; //used to update inventory slot render
    }

    //Key - one inside the office
    if (e.stor.hasItem != -1 &&
        e.stor.collected != 1 &&
        e.stor.type == 3) {
        spriteItemRender(key_img, e.stor.item_xPos, 
                                    e.stor.item_yPos);
    }
    else if (e.stor.collected == 1 && e.stor.type == 3){
        key.collected = 1; //used to update inventory slot render
    }
}

//Render the Item next to the player
void renderPlayerItem(float playerX, float playerY) {
    if (axe.collected) {
        spriteItemRender(axe_img, playerX, playerY);
        //axe range
       /* uncomment to visualize the axe range
        * glPushMatrix();
        glColor3ub(220, 220, 0);
        glTranslatef(itemX, itemY, 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-axe.w, -axe.h);
            glVertex2f(-axe.w,  axe.h);
            glVertex2f( axe.w,  axe.h);
            glVertex2f( axe.w, -axe.h);
       glEnd();
       glPopMatrix();
       renderZombieHitbox();*/
    }
}

//Check if axe is within range to attack zombie
bool zombieAxeCollision(int playerX,int playerY, int id) {
    int hbLeft = playerX;
    int hbRight = playerX + axe.w;
    int hbTop = playerY;
    int hbBottom = playerY + axe.h;

    // External zombie getter and total zombie count
    extern Zombie getZombies(int);
    extern int getVectorSize();
    int zombieTotal = getVectorSize();

    // Iterate through all zombies
    for (int i = 0; i < zombieTotal; i++) {
        Zombie zombie = getZombies(i);

        // Zombie rectangle position and dimensions
        int zPosX = zombie.pos[0];
        int zPosY = zombie.pos[1];
        int zLeft = zPosX;
        int zRight = zPosX + zombie.w;
        int zTop = zPosY;
        int zBottom = zPosY + zombie.h;

        // Check if the zombie is in the same room
        if (zombie.room == id) {
            // Check if the rectangles overlap
            if (hbRight >= zLeft && hbLeft <= zRight &&
                hbBottom >= zTop && hbTop <= zBottom) {
                return true; // Collision detected
            }
        }
    }
    return false; // No collision
}

//reset items if player dies
void resetItems() {
    axe.collected = false;
    healthpack.collected = false;
    key.collected = false;
}
