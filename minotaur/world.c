#include "world.h"
#include "map_data.h"
#include "graphics.h"
#include "player.h"
#include "sound.h"
#include <string.h>
#include <stdlib.h>

int roomsVisited = 0;
int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
int currentWidth = 0;
int currentHeight = 0;
Room* currentRoomPtr = NULL;

extern int room_pool_size;
extern float yaw, pitch, posX, posZ;

void copyToMap(Room source) {
    currentWidth = source.width;
    currentHeight = source.height;

    memset(map, 0, sizeof(map));
    for (int j = 0; j < currentHeight; j++) {
        for (int i = 0; i < currentWidth; i++) {
            map[j][i] = source.data[j][i];
        }
    }

    // Játékos pozicionálása az ajtóhoz
    posX = source.doorX * 2.0f;
    posZ = source.doorZ * 2.0f + 1.0f;

    pitch = 0.0f;
    yaw = 180.0f;
}

void initFirstRoom() {
    roomsVisited = 0;
    currentRoomPtr = &room_start;
    copyToMap(room_start);
    initParticlesEverywhere();
}

void progressToNextLevel() {
    roomsVisited++;
    
    if (roomsVisited >= 10) {
        currentRoomPtr = &room_boss;
        playMusic(1);
    } else {
        int r = rand() % room_pool_size; 
        currentRoomPtr = &room_pool[r];
    }
    
    copyToMap(*currentRoomPtr);
    initParticlesEverywhere();
}