/* Lyanne Rafanan's Source File
 * last updated: 12 NOV 24
 * What's in here?
 *  - function to render Inventory Box
 *
 *
 */
#include "header.h"
void renderInventory(Inventory box, int nbox, int posX, int IBOX) {
    glPushMatrix();
        if (nbox <= IBOX) //boxes
            glColor3ub(34, 34, 59);
        if (nbox > IBOX) //border
            glColor3ub(74, 78, 105);
        glTranslatef(posX, box.pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box.w, -box.h);
            glVertex2f(-box.w,  box.h);
            glVertex2f( box.w,  box.h);
            glVertex2f( box.w, -box.h);
       glEnd();
       glPopMatrix();
}
