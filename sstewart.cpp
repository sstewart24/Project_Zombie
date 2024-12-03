/*  Sophia Stewart
 *   10/08/2024
 *  Purpose: creates a player health bar that increments or decrements based on damage taken or items used
 */
#include "header.h"

const float maxHealth = 180.0f;
const float zAlertHit = 6.0f;
const float heal = 60.0f;
int col_count = 0;

extern Global gl;

//function defs
float increaseHealth(float pHealth);
float damageHealth(float pHealth);
void renderHealth(Health hbox, float pHealth);
void renderPause(Pause p);
bool pCollision(Player player, int);
extern int getVectorSize();
extern void renderLight(int, int);
extern void spriteInit(Sprite& ,std::string);
Sprite playerHealthy;
std::string sprite_image = "./images/Sprite-player.png";

void init_Player_Images(Sprite &sp)
{
    spriteInit(playerHealthy, sprite_image);
    sp.spTex = playerHealthy.spTex;
}

float damageHealth(float pHealth)
{
    pHealth = pHealth - zAlertHit;
    if (pHealth <= 0) {
        pHealth = 0;
    }
    return pHealth;
}

float increaseHealth(float pHealth)
{
    pHealth = pHealth + heal;
    if (pHealth >= 180.0f) {
        pHealth = 180.0f;
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

void renderPause(Pause p) 
{
    glPushMatrix();
    glTranslatef(p.pos[0], p.pos[1], 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glVertex2f(-p.w, -p.h);
        glVertex2f(-p.w,  p.h);
        glVertex2f( p.w,  p.h);
        glVertex2f( p.w, -p.h);
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    
    Rect l, r;
    
    l.bot = p.yres - 20;
    l.left = 10;
    l.center = 0;

    r.bot = p.yres - 20;
    r.left = 450;
    r.center = 0;

    ggprint8b(&l, 420, 0x00ff0000, "3350 - Project_Zombie");
    ggprint8b(&l, 16, 0x00ffff00, "F - Interact with event spaces");
    ggprint8b(&l, 16, 0x00ffff00, "L - Remove border");
    ggprint8b(&l, 16, 0x00ffff00, "E - Remove black square");
    ggprint8b(&r, 420, 0x00ffff00, "");
    ggprint8b(&r, 16, 0x00ffff00, "WASD - Move");
    ggprint8b(&r, 16, 0x00ffff00, "ARROW KEYS - Move");
    ggprint8b(&r, 16, 0x00ffff00, "SPACE - Swing axe?");
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

void spritePlayerRender(Sprite sp, float xPos, float yPos, int direction)
{
    float zPos = 0.0f;
    float cx = sp.xres/6.0;
    float cy = sp.yres;
    int ix = sp.spriteFrame % 6;
    int iy = 0;
    float tx = (float)ix / 6.0;
    float ty = (float)iy;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, sp.spTex.spriteTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);

    glTranslatef(xPos, yPos, zPos);
    
    if (direction) {
        glRotatef(180.0f,1.0f,0.0f,0.0f);
        glRotatef(180.0f,0.0f,0.0f,1.0f);  
    }
    
    glBegin(GL_QUADS);
    glTexCoord2f(tx, ty+1.0);      glVertex2i(-cx/2, -cy/2);
    glTexCoord2f(tx, ty);         glVertex2i(-cx/2, cy/2);
    glTexCoord2f(tx+0.1667, ty);    glVertex2i(cx/2,cy/2);
    glTexCoord2f(tx+0.1667, ty+1.0); glVertex2i(cx/2, -cy/2);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
