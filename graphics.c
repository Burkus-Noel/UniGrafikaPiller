#include "graphics.h"
#include "world.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>

TTF_Font* font = NULL;
float brightness = 1.0f;

extern float yaw;
extern Room* currentRoomPtr;

// --- PARTIKLUK ---
#define MAX_PARTICLES 300
typedef struct {
    float x, y, z, vx, vy, vz, life, homeX, homeZ;
} Particle;

Particle particles[MAX_PARTICLES];

// --- ALAP BEÁLLÍTÁSOK ---

void initText() {
    if (TTF_Init() == -1) return;
    font = TTF_OpenFont("font.ttf", 20);
}

void setupCameraProjection(float fovy, float aspect, float zNear, float zFar) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float f = 1.0f / tanf(fovy * 3.14159265f / 360.0f);
    float m[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar+zNear)/(zNear-zFar), -1,
        0, 0, (2.0f*zFar*zNear)/(zNear-zFar), 0
    };
    glMultMatrixf(m);
    glMatrixMode(GL_MODELVIEW);
}

void setupFog() {
    glEnable(GL_FOG);
    float fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 5.0f);
    glFogf(GL_FOG_END, 25.0f);
}

// --- FÉNYEK ---

void update_lighting() {
    float baseLight = 0.5f * brightness; 
    GLfloat ambient[] = { baseLight, baseLight, baseLight, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    update_lighting();
}

// --- RAJZOLÁS (SEGÉD) ---

void drawWall(float x, float z, float h) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, 1);
        glTexCoord2f(1, h/2); glVertex3f( 1,  h, 1);
        glTexCoord2f(0, h/2); glVertex3f(-1,  h, 1);
        
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0); glVertex3f( 1, -1, -1);
        glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
        glTexCoord2f(1, h/2); glVertex3f(-1,  h, -1);
        glTexCoord2f(0, h/2); glVertex3f( 1,  h, -1);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);
        glTexCoord2f(1, 0); glVertex3f(-1, -1,  1);
        glTexCoord2f(1, h/2); glVertex3f(-1,  h,  1);
        glTexCoord2f(0, h/2); glVertex3f(-1,  h, -1);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0, 0); glVertex3f( 1, -1,  1);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -1);
        glTexCoord2f(1, h/2); glVertex3f( 1,  h, -1);
        glTexCoord2f(0, h/2); glVertex3f( 1,  h,  1);
    glEnd();
    glPopMatrix();
}

void drawDoor(float x, float z, float h, GLuint texture) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0);    glVertex3f(-1.0f, -1.2f, 0.11f); 
        glTexCoord2f(1, 0);    glVertex3f( 1.0f, -1.2f, 0.11f); 
        glTexCoord2f(1, 1.0f); glVertex3f( 1.0f,  h,     0.11f); 
        glTexCoord2f(0, 1.0f); glVertex3f(-1.0f,  h,     0.11f); 
    glEnd();
    glPopMatrix();
}

void drawFire(float x, float z, GLuint fireTexture, GLuint torchTexture) {
    if (fireTexture == 0) return;

    glPushMatrix();
    
    glTranslatef(x, 0.0f, z);
    glRotatef(-yaw, 0, 1, 0);

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    // --- 1. A FÁKLYA NYELE ---
    glBindTexture(GL_TEXTURE_2D, torchTexture);
    glColor3f(brightness * 0.8f, brightness * 0.8f, brightness * 0.8f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.05f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.05f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 3.0f); glVertex3f( 0.05f,  0.6f, 0.0f);
        glTexCoord2f(0.0f, 3.0f); glVertex3f(-0.05f,  0.6f, 0.0f);
    glEnd();

    // --- 2. AZ ANIMÁLT LÁNG ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);
    
    glBindTexture(GL_TEXTURE_2D, fireTexture);

    // Animációs keret kiszámítása (6 képkockás láng)
    int frame = (SDL_GetTicks() / 100) % 6;
    float tMin = frame * (1.0f / 6.0f);
    float tMax = tMin + (1.0f / 6.0f);

    glColor4f(brightness, brightness, brightness, 0.8f);

    glBegin(GL_QUADS);
        glTexCoord2f(tMin, 0.15f); glVertex3f(-0.3f, 0.5f, 0.01f);
        glTexCoord2f(tMax, 0.15f); glVertex3f( 0.3f, 0.5f, 0.01f);
        glTexCoord2f(tMax, 0.85f); glVertex3f( 0.3f, 1.1f, 0.01f);
        glTexCoord2f(tMin, 0.85f); glVertex3f(-0.3f, 1.1f, 0.01f);
    glEnd();

    // Állapotok visszaállítása
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Szín visszaállítása alaphelyzetbe

    glPopMatrix();
}

void drawMinotaur(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(0.4f, 1.2f, 0.4f);
    glColor3f(0.3f, 0.15f, 0.05f);
    drawWall(0, 0, 0); 
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

// --- FŐ RAJZOLÓK ---

void drawMap() {
    glEnable(GL_LIGHTING);
    update_lighting();

    for (int j = 0; j < currentHeight; j++) {
        for (int i = 0; i < currentWidth; i++) {
            float wx = i * 2.0f;
            float wz = j * 2.0f;

            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glTexCoord2f(0, 0); glVertex3f(wx - 1, -1.01f, wz - 1);
                glTexCoord2f(1, 0); glVertex3f(wx + 1, -1.01f, wz - 1);
                glTexCoord2f(1, 1); glVertex3f(wx + 1, -1.01f, wz + 1);
                glTexCoord2f(0, 1); glVertex3f(wx - 1, -1.01f, wz + 1);
            glEnd();

            int cell = map[j][i];
            if (cell == 1) {
                glBindTexture(GL_TEXTURE_2D, stoneTexture);
                drawWall(wx, wz, 60.0f);
            } else if (cell == 2) {
                drawFire(wx, wz, fireTexture, torchTexture);
            } else if (cell == 3 || cell == 4) {
                glPushMatrix();
                glTranslatef(wx, 0, wz);
                if (i == 0 || i == currentWidth - 1) glRotatef(90, 0, 1, 0);
                drawDoor(0, 0, 3.5f, ajtoTexture);
                glTranslatef(0, 3.5f, 0); 
                glBindTexture(GL_TEXTURE_2D, stoneTexture);
                drawWall(0, 0, 25.0f);
                glPopMatrix();
            }
            if (cell == 5 && !minoSpawned) { 
                minoX = wx; minoZ = wz; minoSpawned = true; 
            }
        }
    }
}

void renderText(const char* text, int x, int y, SDL_Color color) {
    if (!font || !text) return;
    SDL_Surface* surf = TTF_RenderUTF8_Solid(font, text, color);
    if (!surf) return;
    SDL_Surface* rgbaSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!rgbaSurf) return;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurf->w, rgbaSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(color.r, color.g, color.b, color.a); 

    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(x, y);
        glTexCoord2f(1,0); glVertex2f(x + rgbaSurf->w, y);
        glTexCoord2f(1,1); glVertex2f(x + rgbaSurf->w, y + rgbaSurf->h);
        glTexCoord2f(0,1); glVertex2f(x, y + rgbaSurf->h);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glDeleteTextures(1, &tex);
    SDL_FreeSurface(rgbaSurf);
}

void drawUI(bool showMenu, bool gameOver) {
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    SDL_Color white = {255, 255, 255, 255}, yellow = {255, 255, 0, 255}, green = {0, 255, 0, 255};
    renderText("Controls: F1", 20, 550, green);

    if (showMenu) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_TEXTURE_2D); 
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f); 
        glBegin(GL_QUADS);
            glVertex2f(50, 50); glVertex2f(750, 50);
            glVertex2f(750, 550); glVertex2f(50, 550);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        renderText("--- CONTROLS ---", 280, 80, yellow);
        renderText("W/S/A/D - Movement", 100, 160, white);
        renderText("SHIFT   - Run", 100, 200, white);
        renderText("C       - Crouch", 100, 240, white);
        renderText("+ / -   - Brightness", 100, 280, white);
        renderText("F1      - Resume", 320, 510, yellow);
    }

    if (gameOver) {
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(0, 0); glVertex2f(800, 0);
            glVertex2f(800, 600); glVertex2f(0, 600);
        glEnd();
        renderText("GAME OVER", 350, 280, (SDL_Color){255, 0, 0, 255});
    }

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
}

// --- PARTICLE ---

void initParticlesEverywhere() {
    if (!currentRoomPtr) return; 
    int count = 0; float coords[200][2];
    for (int j = 0; j < currentRoomPtr->height; j++) {
        for (int i = 0; i < currentRoomPtr->width; i++) {
            if (currentRoomPtr->data[j][i] == 2 && count < 200) {
                coords[count][0] = i * 2.0f; coords[count][1] = j * 2.0f;
                count++;
            }
        }
    }
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (count > 0) {
            int idx = i % count;
            particles[i].homeX = coords[idx][0];
            particles[i].homeZ = coords[idx][1];
            particles[i].life = (float)rand() / RAND_MAX;
        }
    }
}

void updateAndDrawParticles() {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life <= 0) {
            particles[i].x = particles[i].homeX;
            particles[i].y = 1.2f;
            particles[i].z = particles[i].homeZ;
            particles[i].vx = ((rand() % 100) - 50) / 5000.0f;
            particles[i].vz = ((rand() % 100) - 50) / 5000.0f;
            particles[i].vy = ((rand() % 100) / 1000.0f) + 0.005f;
            particles[i].life = 0.8f + (float)(rand() % 40) / 100.0f;
        }
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        particles[i].z += particles[i].vz;
        particles[i].vy -= 0.0005f;
        particles[i].life -= 0.01f;
        if (particles[i].life > 0) {
            glColor4f(1.0f, 0.6f, 0.1f, particles[i].life);
            glVertex3f(particles[i].x, particles[i].y, particles[i].z);
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

GLuint loadTexture(const char* filename) {
    SDL_Surface* surf = IMG_Load(filename);
    if (!surf) return 0;
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    GLint mode = (surf->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h, 0, mode, GL_UNSIGNED_BYTE, surf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surf);
    return tid;
}