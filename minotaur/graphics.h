#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "world.h"

//pi definíció
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// --- Rendszer inicializálás ---
void setupCameraProjection(float fovy, float aspect, float zNear, float zFar);
void initLighting();
void setupFog();
void update_lighting();

// --- Részecskerendszer ---
void initParticlesEverywhere();
void updateAndDrawParticles();

// --- 3D Megjelenítés ---
void drawMap(GameState* state, Assets* assets);
void loadOBJ(const char* filename, SimpleModel* model);
void drawSimpleModel(SimpleModel* model);

// --- Segédfüggvények ---
GLuint loadTexture(const char* filename);

#endif