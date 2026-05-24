#include "world.h"
#include "map_data.h"
#include "graphics.h"
#include "player.h"
#include "sound.h"
#include "map_data.h"
#include <string.h>
#include <stdlib.h>


void copyToMap(Room source, Player* player, MapData* mapData) {
    mapData->currentWidth = source.width;
    mapData->currentHeight = source.height;

    memset(mapData->map, 0, sizeof(mapData->map));
    for (int j = 0; j < mapData->currentHeight; j++) {
        for (int i = 0; i < mapData->currentWidth; i++) {
            mapData->map[j][i] = source.data[j][i];
        }
    }

    player->posX = source.doorX * 2.0f;
    player->posZ = source.doorZ * 2.0f + 1.0f;

    player->pitch = 0.0f;
    player->yaw = 180.0f;
}

void initFirstRoom(Player* player, MapData* mapData) {
    mapData->roomsVisited = 0;
    mapData->currentRoom = getStartRoom(); 
    mapData->currentRoomPtr = &mapData->currentRoom; 
    
    copyToMap(mapData->currentRoom, player, mapData);
    initParticlesEverywhere(mapData);
}

void progressToNextLevel(Player* player, MapData* mapData) {
    mapData->roomsVisited++;
    
    if (mapData->roomsVisited >= 10) {
        mapData->currentRoom = getBossRoom();
        playMusic(1);
    } else {
        mapData->currentRoom = getRandomPoolRoom();
    }
    mapData->currentRoomPtr = &mapData->currentRoom;
    
    copyToMap(mapData->currentRoom, player, mapData);
    initParticlesEverywhere(mapData);
}