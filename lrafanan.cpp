/* Lyanne Rafanan's Source File
 * last updated: 12 NOV 24
 * What's in here?
 *  - function to render Inventory Box
 *
 * Ideas:
 *  - function to position multiple boxes?
 *  - fix math for inventory in asteroids
 *
 */

#include "header.h"
void renderInventory(Inventory box, int nbox, int posX) {
    int red;
    if (nbox > 0 ) {
        red = 0;
    } else {
        red = 74;
    }
    glPushMatrix();
        glColor3ub(red, 78, 105);
        glTranslatef(posX, box.pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-box.w, -box.h);
            glVertex2f(-box.w,  box.h);
            glVertex2f( box.w,  box.h);
            glVertex2f( box.w, -box.h);
       glEnd();
       glPopMatrix();
}
