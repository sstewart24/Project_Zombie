/*  Sophia Stewart
 *   10/08/2024
 *  Purpose: creates a player health bar that increments or decrements based on damage taken or items used
 */
#include "header.h"

const float maxHealth = 180.0f;
const float minHealth = 0.0f;
const float zUnawareHit = 3.0f;
const float zAlertHit = 6.0f;
const float zHostileHit = 9.0f;
const float hBuffWeak = 25.0f;
const float hBuffStrong = 75.0f;
float playerHealth = maxHealth;

//function defs
float updateHealth(float pHealth);
void renderHealth(Health hbox, float pHealth);
bool pCollision(Player player, int);
extern int getVectorSize();

float updateHealth(float pHealth) // <-- Will need save state passed in
{
    pHealth = pHealth - zAlertHit;

    if (pHealth <= 0) {
        pHealth = 0;
        return pHealth;
    }

    return pHealth;
}

void renderHealth(Health hbox, float pHealth)
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


    //updating health bar based on damage
    hbox.h = pHealth;

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

bool pCollision(Player player, int rid) {
    extern Zombie getZombies(int);
    extern int getVectorSize();
    int zombieAmount = getVectorSize();
    
    for (int i = 0; i < zombieAmount; i++) {
        Zombie zom = getZombies(i);
        if (zom.room == rid) {
            if (zom.pos[0] != player.pos[0]) {
                // Calculate the distance between the player and other zombies
                float x = player.pos[0] - zom.pos[0];
                float y = player.pos[1] - zom.pos[1];
                float distance = std::sqrt(x * x + y * y);
                // if the distance is smaller than the threshold, it is a collision
                if (distance < 18.0f) { // the threshold is 18.0f because of the zombies size in render (2 * 9.0f)
                    if (!player.shown) {
                        return false;
                    }
                    if (!zom.alive) {
                        return false;
                    } 

                    printf("Collision!\n");
                    return true;
                }
            }
        }
    }
    return false;
}
