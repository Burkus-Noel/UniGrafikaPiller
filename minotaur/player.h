#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Játékos pozíció és nézőpont
extern float posX, posZ;
extern float yaw, pitch;
extern float eyeHeight; 

// UI és felszerelés állapotok
extern bool showMenu; 
extern bool helmetOn;

// Funkciók
void handlePlayerMovement(const Uint8* state);
void checkSwordPickup();

#endif