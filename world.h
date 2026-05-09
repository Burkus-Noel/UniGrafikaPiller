#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#define MAX_MAP_SIZE 20

typedef struct {
    int width;
    int height;
    int data[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int doorX, doorZ; 
} Room;

// Játéktér állapotai
extern int roomsVisited;
extern int map[MAX_MAP_SIZE][MAX_MAP_SIZE]; 
extern int currentWidth;
extern int currentHeight;

// Függvények
void initFirstRoom();
void progressToNextLevel(); 
void copyToMap(Room source);

#endif