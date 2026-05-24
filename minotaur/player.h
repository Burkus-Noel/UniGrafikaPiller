#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct GameState GameState;
typedef struct MapData MapData;

typedef struct Player {
    float posX;
    float posZ;
    float yaw;
    float pitch;
    float eyeHeight;
} Player;

// Mozgás kezelés, és kard felvételének ellenőrzése
void handlePlayerMovement(Player* player, GameState* state,MapData* mapData,  const Uint8* keyboardState);
void checkSwordPickup(Player* player, GameState* state, MapData* mapData);

#endif