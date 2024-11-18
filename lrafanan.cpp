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

