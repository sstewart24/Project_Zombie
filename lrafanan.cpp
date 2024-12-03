/* Lyanne Rafanan's Source File
 * last updated: 03 DEC 24
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
extern void spriteInit(Sprite& ,std::string); // loads images into the ppm files opengl uses
void init_Item_Images();
void spriteItemRender(Sprite sp, float xPos, float yPos);
void init_inventory();
void renderInventory(Eventspace);
void renderItem(Eventspace);

//Globals
//Healthpack healthpack;
Axe axe;
const int MAXINVENTORY = 4;
Inventory box[MAXINVENTORY];
Inventory bg;

Sprite healthpack_img = Sprite(40, 40); // setup for the item sprites
std::string hp_imagefile = "./images/Health-pack.png"; // string for image
                                                       //
Sprite axe_img = Sprite(40, 40);
std::string axeInv_imagefile = "./images/AxeSprite-Inventory.png";

Sprite key_img = Sprite(40, 40);
std::string keyInv_imagefile = "./images/Sprite-key.png";
Eventspace ev(1, 1, 50.0f, 320.0f, Storage(1,1));

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
        glTranslatef(box[i].pos[0], box[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box[i].w, -box[i].h);
            glVertex2f(-box[i].w,  box[i].h);
            glVertex2f( box[i].w,  box[i].h);
            glVertex2f( box[i].w, -box[i].h);
       glEnd();
       glPopMatrix();
       // i = inventory slot
        if (i==0 && axe.collected == 1)
            spriteItemRender(axe_img, box[i].pos[0], box[i].pos[1]);
        if (i==1 /*&& healthpack.collected*/)
            spriteItemRender(healthpack_img, box[i].pos[0], box[i].pos[1]);
        if (i==2)
            spriteItemRender(key_img, box[i].pos[0], box[i].pos[1]);
    }

}

// Rendering Items to Collect on the Map
void renderItem(Eventspace e) {
    //Axe - inside of main lobby
    if (e.stor.hasItem != -1 && e.stor.collected != 1) {
        spriteItemRender(axe_img, axe.pos[0] , axe.pos[1]);
    }
    else if (e.stor.collected == 1){
        axe.collected = 1;
    }
    //Health packs - Multiple around the maps
    /*if (!healthpack.collected) {
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
    //Key -

}
