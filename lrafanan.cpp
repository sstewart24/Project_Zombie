/* Lyanne Rafanan's Source File
 * last updated: 22 OCT 24
 * What's in here?
 *  - function to render Inventory Box
 *
 */

#include "header.h"
void renderInventory(Inventory box) {
    glPushMatrix();
        glColor3ub(74, 78, 105);
        glTranslatef(box.pos[0], box.pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box.w, -box.h);
            glVertex2f(-box.w,  box.h);
            glVertex2f( box.w,  box.h);
            glVertex2f( box.w, -box.h);
       glEnd();
       glPopMatrix();
}
