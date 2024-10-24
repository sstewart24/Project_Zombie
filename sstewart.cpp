/*  Sophia Stewart
*   10/08/2024
*  Purpose: creates a player health bar that increments or decrements based on damage taken or items used
*/
#include "header.h"

const float maxHealth = 200.0f;
const float minHealth = 0.0f;
const float zUnawareHit = 15.0f;
const float zAlertHit = 35.0f;
const float zHostileHit = 50.0f;
const float hBuffWeak = 25.0f;
const float hBuffStrong = 75.0f;

//function defs
//double get_health();
float updateHealth(void);
void renderHealth(Health hbox);

//double player_health = get_health();

float updateHealth(void) // <-- Will need save state passed in
{
    int playerHealth = maxHealth;
    // Will load player's health on loading game save state,
    // if a save state has not been created or a new game is launched
    // player's health will start at max health
    
    
    
    return playerHealth;
}

void renderHealth(Health hbox)
{
    //box surrounding health bar
    glPushMatrix();
        glColor3ub(252, 252, 252);
        glTranslatef(hbox.basepos[0], hbox.basepos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-hbox.basew, -hbox.baseh);
            glVertex2f(-hbox.basew,  hbox.baseh);
            glVertex2f( hbox.basew,  hbox.baseh);
            glVertex2f( hbox.basew, -hbox.baseh);
        glEnd();
        glPopMatrix();
    
    //box behind health bar to show health depletion
    glPushMatrix();
        glColor3ub(36, 36, 36);
        glTranslatef(hbox.backpos[0], hbox.backpos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-hbox.backw, -hbox.backh);
            glVertex2f(-hbox.backw,  hbox.backh);
            glVertex2f( hbox.backw,  hbox.backh);
            glVertex2f( hbox.backw, -hbox.backh);
        glEnd();
        glPopMatrix();
        
    //health bar box    
    glPushMatrix();
        glColor3ub(23, 252, 42);
        glTranslatef(hbox.pos[0], hbox.pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-hbox.w, -hbox.h);
            glVertex2f(-hbox.w,  hbox.h);
            glVertex2f( hbox.w,  hbox.h);
            glVertex2f( hbox.w, -hbox.h);
        glEnd();
        glPopMatrix();
}
