#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "world.h"
#include "player.h"


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void setupCameraProjection(float fovy, float aspect, float zNear, float zFar);
void initLighting();
void setupFog();
void update_lighting();

void initParticlesEverywhere(MapData* mapData);
void updateAndDrawParticles();

void drawMap(GameState* state, Assets* assets, Player* player, MapData* mapData);;
void loadOBJ(const char* filename, SimpleModel* model);
void drawSimpleModel(SimpleModel* model);

GLuint loadTexture(const char* filename);

#endif