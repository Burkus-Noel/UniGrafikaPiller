#define _USE_MATH_DEFINES
#include "player.h"
#include "world.h"
#include "sound.h"
#include "engine.h"
#include <math.h>
#include <SDL2/SDL.h>

// Állapotváltozók


extern float brightness;

void checkSwordPickup(Player* player, GameState* state, MapData* mapData) {
    // Most már a player struktúrából olvassa a pozíciót
    int gridX = (int)((player->posX + 1.0f) / 2.0f);
    int gridZ = (int)((player->posZ + 1.0f) / 2.0f);

    if (gridX >= 0 && gridX < mapData->currentWidth && gridZ >= 0 && gridZ < mapData->currentHeight) {
        if (mapData->map[gridZ][gridX] == 6) {
            mapData->map[gridZ][gridX] = 0;
            state->swordCount++; // Beírja a GameState-be a talált kardot
        }
    }
}

// Ütközésvizsgálat
int isWall(float x, float z, MapData* mapData) {
    int i = (int)((x + 1.0f) / 2.0f);
    int j = (int)((z + 1.0f) / 2.0f);

    if (i < 0 || i >= mapData->currentWidth || j < 0 || j >= mapData->currentHeight) return 1;

    int cell = mapData->map[j][i];
    return (cell == 1 || cell == 4);
}

void handlePlayerMovement(Player* player, GameState* state, MapData* mapData, const Uint8* keyboardState) {

    float speed = (keyboardState[SDL_SCANCODE_LSHIFT]) ? 0.12f : 0.06f;
    
    // Guggolás kezelése a struktúrában
    if (keyboardState[SDL_SCANCODE_C]) {
        if (player->eyeHeight < -0.4f) player->eyeHeight += 0.08f; 
    } else {
        if (player->eyeHeight > -1.0f) player->eyeHeight -= 0.08f;
    }

    // Irányszámítás a játékos yaw szögéből
    float rad = player->yaw * (float)M_PI / 180.0f;
    float fx = sinf(rad), fz = -cosf(rad);
    float rx = cosf(rad), rz = sinf(rad);

    float moveX = 0, moveZ = 0;
    if (keyboardState[SDL_SCANCODE_W]) { moveX += fx; moveZ += fz; }
    if (keyboardState[SDL_SCANCODE_S]) { moveX -= fx; moveZ -= fz; }
    if (keyboardState[SDL_SCANCODE_D]) { moveX += rx; moveZ += rz; }
    if (keyboardState[SDL_SCANCODE_A]) { moveX -= rx; moveZ -= rz; }

    // Ütközésvizsgálat és mozgás
    if (moveX != 0 || moveZ != 0) {
        float nextX = player->posX + moveX * speed;
        float nextZ = player->posZ + moveZ * speed;
        float off = 0.3f; 

        if (!isWall(nextX + (moveX > 0 ? off : -off), player->posZ, mapData)) player->posX = nextX;
        if (!isWall(player->posX, nextZ + (moveZ > 0 ? off : -off), mapData)) player->posZ = nextZ;
    }

    // Ajtón való áthaladás
    int curI = (int)((player->posX + 1.0f) / 2.0f);
    int curJ = (int)((player->posZ + 1.0f) / 2.0f);

    if (mapData->map[curJ][curI] == 3)  {
    progressToNextLevel(player, mapData);
    playDoorSound();
    }

    // Fényerő állítás
    if (keyboardState[SDL_SCANCODE_KP_PLUS]  && brightness < 5.0f)  brightness += 0.05f;
    if (keyboardState[SDL_SCANCODE_KP_MINUS] && brightness > -0.5f) brightness -= 0.05f;
}