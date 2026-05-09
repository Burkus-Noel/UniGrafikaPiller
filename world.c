#include "world.h"
#include "map_data.h"
#include "graphics.h"
#include "player.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int roomsVisited = 0;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int currentWidth = 20;
int currentHeight = 20;

// Ezt kérted vissza
Room* currentRoomPtr = NULL;

extern int room_pool_size;
extern float yaw, pitch;
extern float posX, posZ;

float limitX, limitZ; 

void copyToMap(Room source) {
    currentWidth = source.width;
    currentHeight = source.height;

    limitX = (currentWidth * 2.0f) - 1.5f;
    limitZ = (currentHeight * 2.0f) - 1.5f;

    memset(map, 0, sizeof(map));
    for (int j = 0; j < currentHeight; j++) {
        for (int i = 0; i < currentWidth; i++) {
            map[j][i] = source.data[j][i];
        }
    }

    posX = source.doorX * 2.0f;
    posZ = source.doorZ * 2.0f + 1.2f;

    pitch = 0.0f; 
    yaw = 180.0f; 
}

void initFirstRoom() {
    roomsVisited = 0;
    currentRoomPtr = &room_start; // Beállítás
    copyToMap(room_start);
    initParticlesEverywhere();
}

void progressToNextLevel() {
    roomsVisited++;
    
    if (roomsVisited >= 10) {
        currentRoomPtr = &room_boss;
    } else {
        int r = rand() % room_pool_size; 
        currentRoomPtr = &room_pool[r];
    }
    
    copyToMap(*currentRoomPtr); // A mutató alatti adatot másoljuk
    initParticlesEverywhere();
}