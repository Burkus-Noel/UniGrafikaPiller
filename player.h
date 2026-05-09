#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Játékos adatok
extern float posX, posZ;
extern float yaw, pitch;
extern float eyeHeight; 
extern bool showMenu; 

void handlePlayerMovement(const Uint8* state);

#endif