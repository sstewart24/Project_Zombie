/*  Sophia Stewart
*   10/08/2024
*  Purpose: creates a player health bar that increments or decrements based on damage taken or items used
*/
#include "header.h"

#define MAX_HEALTH 200
#define MIN_HEALTH 0
#define ZOMBIE_UNAWARE 15
#define ZOMBIE_ALERT 35
#define ZOMBIE_HOSTILE 50
#define HBUFF_WEAK 25
#define HBUFF_STRONG 75

//function defs
//double get_health();
//double health_modifier(double player_health);
void renderHealth(Health hbox);

//double player_health = get_health();

/*double get_health() // <-- Will need save state passed in
{
    // Will load player's health on loading game save state,
    // if a save state has not been created or a new game is launched
    // player's health will start at max health

    player_health = 200;
    return player_health;
}*/

void renderHealth(Health hbox)
{
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
