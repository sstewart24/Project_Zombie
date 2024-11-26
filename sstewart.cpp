/*  Sophia Stewart
 *   10/08/2024
 *  Purpose: creates a player health bar that increments or decrements based on damage taken or items used
 */
#include "header.h"

const float maxHealth = 180.0f;
const float zAlertHit = 6.0f;
const float hBuff = 60.0f;
int col_count = 0;

//function defs
float updateHealth(float pHealth);
void renderHealth(Health hbox, float pHealth);
bool pCollision(Player player, int);
extern int getVectorSize();

extern void spriteInit(Sprite& ,std::string);
Sprite playerHealthy;
std::string sprite_image = "./images/Player-sprite-ss.png";

void init_Player_Images(Sprite &sp)
{
    spriteInit(playerHealthy, sprite_image);
    sp.spTex = playerHealthy.spTex;
}

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
                    col_count++;
                    if (!player.shown) {
                        return false;
                    }
                    if (!zom.alive) {
                        return false;
                    } 

                    //buffer to give player damage cooldown
                    if (col_count == 1) {
                    return true;
                    } 

                    if (col_count > 1 && col_count < 10) {
                    return false;
                    } 

                    if (col_count == 10) {
                    col_count = 0;
                    return true;
                    }
                }
            }
        }
    }
    return false;
}

void spritePlayerRender(Sprite sp, float xPos, float yPos)
{

    float zPos = 0.0f;
    float cx = sp.xres/8.0;
    float cy = sp.yres;

    int ix = sp.spriteFrame % 8;
    int iy = 0;
    float tx = (float)ix / 8.0;
    float ty = (float)iy;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
    //
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    /*
       if (sp.spriteFrame >= 8)
       iy = 1;
       */

    glTranslatef(xPos, yPos, zPos);
    glRotatef(0.0f,0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(tx, ty+1.0);      glVertex2i(0, 0);
    glTexCoord2f(tx, ty);         glVertex2i(0, cy);
    glTexCoord2f(tx+0.125, ty);    glVertex2i(cx,cy);
    glTexCoord2f(tx+0.125, ty+1.0); glVertex2i(cx, 0);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
