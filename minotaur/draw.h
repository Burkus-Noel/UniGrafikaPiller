#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL_opengl.h>
#include "world.h"

void drawWall(float x, float z, float h);
void drawDoor(float x, float z, float h, GLuint texture);
void drawFire(float x, float z, GLuint texture, GLuint frameTexture, struct Player* player);
void drawMinotaur(float x, float z, SimpleModel* model, struct Player* player);

#endif