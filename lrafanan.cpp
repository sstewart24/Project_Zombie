/* Lyanne Rafanan's Source File
 * last updated: 28 NOV 24
 * What's in here?
 *  - Render and initialize values for Inventory Box
 *  - Render Collectable Items:
 *      - axe(1): stun/kill zombies
 *      - healthpack(in progress, multiple around the map): incr player health
 * TODO:
 * - add more items:
 *      - cure: unzombie the zombies
 *      - map pieces? : collect to find the cure
 * - fix item alignment inside of inventory slots
 * */
#include "header.h"
// Funtion Prototypes:
extern void spriteInit(Sprite& ,std::string); // loads images into the ppm
                                              // files opengl uses
void init_Item_Images();
void spriteItemRender(Sprite sp, float xPos, float yPos);
void init_inventory();
void renderInventory();
void renderItem(Player player);

//Globals
//Healthpack healthpack;
Axe axe;
const int MAXINVENTORY = 4;
Inventory box[MAXINVENTORY];
Inventory bg;
Sprite healthpack_img = Sprite(40, 40); // setup for the item sprites
std::string hp_imagefile = "./images/Health-pack.png"; // string for where the
                                                       // image is
Sprite axe_Inventory = Sprite(40, 40);
std::string axeInv_imagefile = "./images/AxeSprite-Inventory.png";

Sprite key_Inventory = Sprite(40, 40);
std::string keyInv_imagefile = "./images/Sprite-key.png";

// Initializing Images for render
void init_Item_Images() {
    // One instance of an image for the inventory
    spriteInit(healthpack_img, hp_imagefile);
    spriteInit(axe_Inventory, axeInv_imagefile);
    spriteInit(key_Inventory, keyInv_imagefile);
}

// Render the item into an inventory slot, called in renderInventory()
//TODO: Fix item alignment
void spriteItemRender(Sprite sp, float xPos, float yPos) {
    float zPos = 0.0f;
    float cx = sp.xres / 2;
        float cy = sp.yres / 2;

    float tx = 0.0;
        float ty = 0.0;

        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
        //
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);

    glTranslatef(xPos, yPos, zPos);
    //glRotatef(0.0f,0.0f,0.0f,0.0f);
        glBegin(GL_QUADS);
                glTexCoord2f(tx, ty+1.0);      glVertex2i(-cx, -cy);
                glTexCoord2f(tx, ty);         glVertex2i(-cx, cy);
                glTexCoord2f(tx+1.0, ty);    glVertex2i(cx, cy);
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
        /*if (nbox > IBOX) //border
            glColor3ub(74, 78, 105);*/
        glTranslatef(box[i].pos[0], box[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box[i].w, -box[i].h);
            glVertex2f(-box[i].w,  box[i].h);
            glVertex2f( box[i].w,  box[i].h);
            glVertex2f( box[i].w, -box[i].h);
       glEnd();
       glPopMatrix();
       // i = inventory slot
        if (i==0 && axe.collected)
            spriteItemRender(axe_Inventory, box[i].pos[0], box[i].pos[1]);
        if (i==1 /*&& healthpack.collected*/)
            spriteItemRender(healthpack_img, box[i].pos[0], box[i].pos[1]);
        if (i==2)
            spriteItemRender(key_Inventory, box[i].pos[0], box[i].pos[1]);
    }

}

// Rendering Items to Collect on the Map
//TODO: Make it work for multiple items
int xres = 640;
int yres = 480;
void renderItem(Player player, Room currentRoom) {
    //Axe - inside of main lobby
    axe.pos[0] = (640 / 2) + 50;
    axe.pos[1] = (480 / 2);
    if (!axe.collected  && axe.room == currentRoom.id) {
        spriteItemRender(axe_Inventory, axe.pos[0], axe.pos[1]);
    }
    if ((player.pos[0] == axe.pos[0]) &&
         (player.pos[1] == axe.pos[1])) {
        axe.collected = true;
        axe.available += 1;
        printf("axe collected\n");
    }
   /*
    //Health pack
    if (!healthpack.collected) {
        healthpack.pos[0] = (640 / 2) + 50;
        healthpack.pos[1] = (480 / 2) - 30;
        spriteItemRender(healthpack_img, healthpack.pos[0],
                         healthpack.pos[1]);
           }
    if ((player.pos[0] == healthpack.pos[0]) &&
         (player.pos[1] == healthpack.pos[1])) {
        healthpack.collected = true;
        healthpack.available += 1;
        printf("health pack collected\n");
    }
    */
}
