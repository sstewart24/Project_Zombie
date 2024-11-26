/* Lyanne Rafanan's Source File
 * last updated: 18 NOV 24
 * What's in here?
 *  - function to render and initialize values for Inventory Box
 *  - function to render Collectable Items (just an axe for right now)
 *
 * Ideas:
 *  - When itemA pos = player pos and itemA room = player room:
 *      collected = true and +1 itemA into inventory
 *
 */
#include "header.h"
const int MAXINVENTORY = 4;
Inventory box[MAXINVENTORY];
Inventory bg;

// Help load images into the ppm files opengl uses
extern void spriteInit(Sprite& ,std::string);

// This would be the setup for the item sprites
// Give a sprite, then a string for where the image is
Sprite healthpack = Sprite(40, 40);
std::string hp_imagefile = "./images/Health-pack.png";
Sprite axe_Inventory = Sprite(40, 40);
std::string axeInv_imagefile = "./images/AxeSprite-Inventory.png";

void init_Item_Images()
{
    // One instance of an image for the inventory
    // For more items, just add another spriteInit()
    // With their respective item
    spriteInit(healthpack, hp_imagefile);
    spriteInit(axe_Inventory, axeInv_imagefile);
}

// Render the item into an inventory slot
// This needs a bit of tinkering since the item(s) are not aligned
void spriteItemRender(Sprite sp, float xPos, float yPos)
{
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

void renderInventory() {
    init_inventory();
//background box -----------------------------------------------------
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
//main boxes ---------------------------------------------------------
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
        if (i==0) // Which inventory slot it is in
            spriteItemRender(axe_Inventory, box[i].pos[0], box[i].pos[1]); // where spriteItemrender is called
        if (i==1) 
            spriteItemRender(healthpack, box[i].pos[0], box[i].pos[1]); // where spriteItemrender is called
    }

}

//TODO: change later to work for multiple items
void renderItem(Axe axe) {
    if (!axe.collected) {
    glPushMatrix();
            glColor3ub(0, 0, 0);
        glTranslatef(axe.pos[0], axe.pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-axe.w, -axe.h);
            glVertex2f(-axe.w,  axe.h);
            glVertex2f( axe.w,  axe.h);
            glVertex2f( axe.w, -axe.h);
       glEnd();
       glPopMatrix();
    }
}

