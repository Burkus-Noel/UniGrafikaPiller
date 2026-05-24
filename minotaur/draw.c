#define _USE_MATH_DEFINES
#include "graphics.h"
#include "world.h"
#include "player.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>


void drawWall(float x, float z, float h) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, 1);
        glTexCoord2f(1, h/2); glVertex3f( 1, h, 1);
        glTexCoord2f(0, h/2); glVertex3f(-1, h, 1);
        
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0); glVertex3f( 1, -1, -1);
        glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
        glTexCoord2f(1, h/2); glVertex3f(-1, h, -1);
        glTexCoord2f(0, h/2); glVertex3f( 1, h, -1);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
        glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
        glTexCoord2f(1, h/2); glVertex3f(-1, h, 1);
        glTexCoord2f(0, h/2); glVertex3f(-1, h, -1);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f( 1, -1, 1);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -1);
        glTexCoord2f(1, h/2); glVertex3f( 1, h, -1);
        glTexCoord2f(0, h/2); glVertex3f( 1, h, 1);
    glEnd();
    glPopMatrix();
}

void drawDoor(float x, float z, float h, GLuint texture) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1); // Az ajtó előlapja felénk néz
        glTexCoord2f(0, 0);    glVertex3f(-1.0f, -1.2f, 0.11f); 
        glTexCoord2f(1, 0);    glVertex3f( 1.0f, -1.2f, 0.11f); 
        glTexCoord2f(1, 1.0f); glVertex3f( 1.0f,  h,     0.11f); 
        glTexCoord2f(0, 1.0f); glVertex3f(-1.0f,  h,     0.11f); 
    glEnd();
    
    glPopMatrix();
}

void drawFire(float x, float z, GLuint fireTex, GLuint torchTex, Player* player) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(-player->yaw, 0, 1, 0);
    glEnable(GL_TEXTURE_2D);

    // 1. FÁKLYA NYELE
    glBindTexture(GL_TEXTURE_2D, torchTex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-0.05f, -1.0f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 0.05f, -1.0f, 0.0f);
        glTexCoord2f(1, 3); glVertex3f( 0.05f,  0.6f, 0.0f);
        glTexCoord2f(0, 3); glVertex3f(-0.05f,  0.6f, 0.0f);
    glEnd();

    // 2. LÁNG
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    
    glBindTexture(GL_TEXTURE_2D, fireTex);

    int frame = (SDL_GetTicks() / 100) % 6;
    float tMin = frame * (1.0f / 6.0f);
    float tMax = tMin + (1.0f / 6.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(tMax, 1.0f); glVertex3f( 0.3f, 0.5f, 0.01f);
        glTexCoord2f(tMin, 1.0f); glVertex3f( 0.3f, 1.1f, 0.01f);
        glTexCoord2f(tMin, 0.0f); glVertex3f(-0.3f, 1.1f, 0.01f);
        glTexCoord2f(tMax, 0.0f); glVertex3f(-0.3f, 0.5f, 0.01f);
    glEnd();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawMinotaur(float x, float z, SimpleModel* model, Player* player) {
    glPushMatrix();
        glTranslatef(x, 1.2f, z); 
        float dx = player->posX - x;
        float dz = player->posZ - z;
        float angle = atan2f(dx, dz) * (180.0f / M_PI);
        glRotatef(angle, 0, 1, 0); 
        glScalef(2.0f, 2.0f, 2.0f); 
        drawSimpleModel(model);
    glPopMatrix();
}