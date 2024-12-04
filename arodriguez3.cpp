/*
 *=================================================================
 * Adrian's source file
 *=================================================================
 */
#include "header.h"
using namespace std;

#define MOVESTEP 0.5f // How fast the zombie moves

void renderZombieDetection(int, int, int);
extern int checkWall(float*, Room);

// Sprite the zombies will use
extern void spriteInit(Sprite& ,std::string);
Sprite zombieSprite = Sprite(192, 64, 0);
std::string sprite_image_Z = "./images/Sprite-zombie.png";
Sprite detectionSprite = Sprite(48, 16, 0);
std::string sprite_z_detect = "./images/Sprite-detection.png";

void init_zomb_Sprites()
{
	spriteInit(zombieSprite, sprite_image_Z);
	zombieSprite.delay = 0.2;
	spriteInit(detectionSprite, sprite_z_detect);
}
Timers ztimer;

int Zcol_count = 0; // For zombie collision countdown

// Vector for Zombies 
std::vector<Zombie> default_zombies = {
	Zombie(0, 575.0f, 400.0f, 0, 1, 0), // Entrance
	Zombie(1, 75.0f, 100.0f, 0, 1, 0),
	Zombie(2, 575.0f, 100.0f, 0, 1, 0),
	Zombie(3, 75.0f, 150.0f, 0, 1, 0),
	
	Zombie(4, 200.0f, 280.0f, 0, 1, 1), // Office

	Zombie(5, 500.0f, 280.0f, 0, 1, 2), // Hall

	Zombie(6, 555.0f, 350.0f, 0, 1, 4), // Lab
	Zombie(7, 120.0f, 350.0f, 0, 1, 4),
	
	Zombie(8, 200.0f, 260.0f, 0, 1, 5),	// Restroom

	Zombie(9, 575.0f, 400.0f, 0, 1, 6), // Testlab/Starting room
	Zombie(10, 80.0f, 100.0f, 0, 1, 6),
	Zombie(11, 90.0f, 380.0f, 0, 1, 6)
};

std::vector<Zombie> zombies;
int zombieamount = 12;
void init_zombie()
{
	int i = 0;
	while (i < zombieamount) {
		zombies.push_back(default_zombies[i]);
		i++;
	}
}

void clear_zombie ()
{
	int i = 0;
	while (i < zombieamount) {
		zombies.pop_back();
		i++;
	}
}

int getVectorSize()
{ // function for Sophia
	return zombies.size(); 
}

Zombie getZombies(int index)
{ // function for Sophia
	return zombies[index];
}

// Function to get a random direction
int getRandomDirection()
{
    return rand() % 4; // Returns a random integer between 0 and 3
}

bool Zcollision(float newPos[2], const Zombie& zombie)
{ // newPos is the other zombie
	for (size_t i = 0; i < zombies.size(); i++) {
		if (zombies[i].id != zombie.id) {
			// Calculate the distance between the zombie and other zombies
			float x = newPos[0] - zombies[i].pos[0];
			float y = newPos[1] - zombies[i].pos[1];
			float distance = sqrt(x * x + y * y);
			
			// If the distance is smaller than the threshold, it is a collision
			if (distance < 18.0f) { // the threshold is 18.0f (2 * 9.0f)
				Zcol_count++;
				if (!zombie.alive) {
					return false;
				}
				if (distance == 0.0f) {
					return false; // No collision
				}
				if (Zcol_count == 0) {
					return true;
				}
				if (Zcol_count >= 1 && Zcol_count < 30) {
					//printf("Counting: %i\n", count);
					return false;
				}
				if (Zcol_count == 30) {
					Zcol_count = 0;
					return true;
				}
                return true; // Collission
			}
		}
	}
	return false; // No collision
}

bool Zfollow(Zombie& zombie, Player& player, Room current)
{
	float follow_range = 100.0f;
	float Ppos[2] = {player.pos[0], player.pos[1]}; 	// Player position
	float Zpos[2] = {zombie.pos[0], zombie.pos[1]}; 	// Zombie position

	// Calculate the distance between the zombie and the player
	float dx = Ppos[0] - Zpos[0];
	float dy = Ppos[1] - Zpos[1];
	float distance = sqrt(dx * dx + dy * dy);

	// If the player is shown and zombie is alive, follow
	if (player.shown && zombie.alive) {
		// If the zombie was following the player and the player is out of range
		if (zombie.following && distance >= follow_range) {
			zombie.count = 0;			// reset count
			zombie.waiting = 1;			// reset waiting
			zombie.following = false;	// stopped following
			//printf("Counter reset\n");
			return false;
		}
		// If the zombie is within the range of the player, follow the player
		if (distance < follow_range) {
			if(!zombie.waiting) {
				// Calculate the direction towards the player
				// Normalize the direction vector (dx, dy)
				float Xdir = dx / distance;
				float Ydir = dy / distance;

				// Update zombie position toward the player
				float NewZpos[2];
				NewZpos[0] = zombie.pos[0] + MOVESTEP * Xdir;
				NewZpos[1] = zombie.pos[1] + MOVESTEP * Ydir;

				// Checks to see if the Zombie is colliding with a wall
				if (!checkWall(NewZpos, current)) {
					zombie.pos[0] = NewZpos[0];
					zombie.pos[1] = NewZpos[1];

				}
				renderZombieDetection(2, zombie.pos[0], zombie.pos[1] + 50);
				zombie.following = true; 	// marked as following
				return true; 				// Zombie follows player
			} else {
				zombie.count++;
				if (zombie.count > 1 && zombie.count < 30) {
					//printf("Counting: %i\n", zombie.count);
					renderZombieDetection(1, zombie.pos[0], zombie.pos[1] + 50);
					return false;
				}
				if (zombie.count == 30) {
					//printf("Count is now: %i\n", zombie.count);
					zombie.waiting = 0;
				}
			}
		}
		if (zombie.waiting && distance > follow_range) {
			// Resets timer when player leaves range
			zombie.count = 0;
			//printf("Out of range, Counter reset\n");
		}
	}
	else if (!player.shown && zombie.following) { 
		// Resets everything when player hides
		zombie.count = 0;
		zombie.waiting = 1;
		zombie.following = false;
		//printf("Counter reset\n");
	}
	return false; // Zombie does not follow player
}

void Zroam(Zombie& zombie, Room current)
{
    // Float moveStep = 2.0f; // how much to move each update
    float newPos[2] = {zombie.pos[0], zombie.pos[1]};
	
    // Update position based on current direction
    switch (zombie.direction) {
        case 0: // Moving up
			//Zwait();
            newPos[1] += MOVESTEP;
			zombie.angle = 360.0f;
            break;
        case 1: // Moving right
            newPos[0] += MOVESTEP;
			zombie.angle = 270.0f;
            break;
        case 2: // Moving down
            newPos[1] -= MOVESTEP;
			zombie.angle = 180.0f;
            break;
        case 3: // Moving left
            newPos[0] -= MOVESTEP;
			zombie.angle = 90.0f;
            break;
    }

	// Update the distance moved
    zombie.moveDistance += MOVESTEP;

    // Check if max distance has been reached to turn around Zombie
    if (zombie.moveDistance >= zombie.maxDistance) {
        zombie.direction = getRandomDirection(); 	// change direction randomly
        zombie.moveDistance = 0; 					// Reset distance moved

    } else if (checkWall(newPos, current)) {
        // Collision detected with wall, Zombie turns around
        zombie.direction = getRandomDirection(); 
        zombie.moveDistance = 0; 
		return;

    } else if (Zcollision(newPos, zombie)) {
		// Collision detected with another zombie, turn around
        zombie.direction = getRandomDirection();
        zombie.moveDistance = 0;
        return; 

	} else if (zombie.count != 0) {
		// Check if zombie is suspicious of player, stop moving
		zombie.moveDistance = 0;
		//printf("Zombie paused\n");
	} else {
        // Update position only if there's no collision
        zombie.pos[0] = newPos[0];
        zombie.pos[1] = newPos[1];
    }
}

//Drawing/Rendering Zombies
void renderZombie(Room current, Player player)
{
	ztimer.recordTime(&ztimer.timeCurrent);
		double timeSpan = ztimer.timeDiff(&ztimer.walkTime, &ztimer.timeCurrent);
		if (timeSpan > zombieSprite.delay) {
			//advance
			++zombieSprite.spriteFrame;
			if (zombieSprite.spriteFrame >= 8)
				zombieSprite.spriteFrame = 0;
			ztimer.recordTime(&ztimer.walkTime);
		} 
	//zombieSprite.spriteFrame += 1;

	// Uses for loop to make zombies based on the size of the zombie vector
	for (size_t i = 0; i < zombies.size(); i++) {
		
		// If the zombies room # matches the current room id, draw the zombie
		if (zombies[i].room == current.id) {
            if (zombies[i].alive == 1) {
				if (!Zfollow(zombies[i], player, current)) {
					Zroam(zombies[i], current);
				}
				
				//draws the zombies
				glPushMatrix();
				glTranslatef(zombies[i].pos[0], zombies[i].pos[1], 0.0f);
				glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
				float size = 9.0f; // size of zombie
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBegin(GL_QUADS);
					glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
					glVertex3f(size, size, 0.0f); 	// top right
					glVertex3f(size, -size, 0.0f); 	// bottom right
					glVertex3f(-size, -size, 0.0f); // bottom left
					glVertex3f(-size, size, 0.0f); 	// top left
				glEnd();

				glBegin(GL_POINTS);
				glVertex2f(0.0f, 0.0f);
				glEnd();
				glPopMatrix();

				glDisable(GL_BLEND);

				float zPos = 0.0f;
    			float cx = zombieSprite.xres/6.0;
    			float cy = zombieSprite.yres;

    			int ix = zombieSprite.spriteFrame % 6;
    			int iy = 0;
    			float tx = (float)ix / 6.0;
    			float ty = (float)iy;

    			glPushMatrix();
    			glColor3f(1.0, 1.0, 1.0);
    			glBindTexture(GL_TEXTURE_2D, zombieSprite.spTex.spriteTexture);
    			//
    			glEnable(GL_ALPHA_TEST);
    			glAlphaFunc(GL_GREATER, 0.0f);
    			glColor4ub(255,255,255,255);

    			glTranslatef(zombies[i].pos[0], zombies[i].pos[1], zPos);
    			if (zombies[i].direction == 3) {
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
		}
	}
}

// Takes the zombie position and puts this ontop of 
// their head when within range of player for detection
// 
void renderZombieDetection(int detection_state, int xPos, int yPos)
{
	float zPos = 0.0f;
    float cx = detectionSprite.xres/3.0f;
    float cy = detectionSprite.yres;

	int ix = detection_state % 3;
	int iy = 0;
	float tx = (float)ix / 3.0f;
	float ty = (float)iy;

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, detectionSprite.spTex.spriteTexture);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);

	glTranslatef(xPos, yPos, zPos);
	// May need to mess with the Vertex cords to get them to be above the head
	glBegin(GL_QUADS);
		glTexCoord2f(tx, ty+1.0);      glVertex2i(-cx/2, -cy/2);
		glTexCoord2f(tx, ty);         glVertex2i(-cx/2, cy/2);
		glTexCoord2f(tx+0.33333, ty);    glVertex2i(cx/2,cy/2);
		glTexCoord2f(tx+0.33333, ty+1.0); glVertex2i(cx/2, -cy/2);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
