#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// --- Erőforrások (Textúrák) ---
extern GLuint stoneTexture;
extern GLuint fireTexture;
extern GLuint ajtoTexture;
extern GLuint floorTexture;
extern GLuint torchTexture;

// --- Külső állapotváltozók ---
extern float minoX, minoZ;
extern bool minoSpawned;

// --- Rendszer inicializálás ---
void setupCameraProjection(float fovy, float aspect, float zNear, float zFar);
void initLighting();
void initText();
void setupFog();

// --- Játékmenet és effektek ---
void initParticlesEverywhere();
void updateAndDrawParticles();
void update_lighting();

// --- 3D Megjelenítés ---
void drawMap();
void drawDoor(float x, float z, float h, GLuint texture);
void drawWall(float x, float z, float h);
void drawFire(float x, float z, GLuint texture, GLuint frameTexture);
void drawMinotaur(float x, float z);

// --- 2D UI és Szöveg ---
void drawUI(bool showMenu, bool gameOver);
void renderText(const char* text, int x, int y, SDL_Color color);

// --- Segédfüggvények ---
GLuint loadTexture(const char* filename);

#endif