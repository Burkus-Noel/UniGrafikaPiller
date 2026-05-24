#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include <math.h>
#include "player.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#define MAX_MAP_SIZE 20

typedef struct Player Player;


// Modell betöltéshez szükséges kódok
#define MAX_VERTICES 500000
#define MAX_FACES 1000000

typedef struct { float x, y, z; } ObjVector3;
typedef struct { float u, v; } ObjVector2;
typedef struct { int v, vt; } ObjFace;

typedef struct {
    ObjVector3 vertices[MAX_VERTICES]; 
    ObjVector2 texcoords[MAX_VERTICES];
    ObjFace faces[MAX_FACES];       
    int numVertices, numTexCoords, numFaces;
    GLuint texture;
} SimpleModel;

// Szoba struktúra
typedef struct {
    int width;
    int height;
    int data[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int doorX, doorZ; 
} Room;

// Játék állapot
typedef struct GameState {
    bool running;
    bool gameOver;
    bool gameWon;
    int swordCount;
    float minoX, minoZ;
    float minoSpeed;
    bool minoSpawned;
    bool helmetOn;
    bool showMenu;
} GameState;

//TEXTÚRÁK és modellek
typedef struct {
    GLuint stone;
    GLuint ajto;
    GLuint fire;
    GLuint floor;
    GLuint torch;
    GLuint helmet;
    GLuint sword;
    SimpleModel minoModel;
} Assets;


typedef struct MapData {
    int roomsVisited;
    int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int currentWidth;
    int currentHeight;
    Room currentRoom;
    Room* currentRoomPtr;
} MapData;


// Pályakezelés
void initFirstRoom(Player* player,MapData* mapData);
void progressToNextLevel(Player* player,MapData* mapData); 
void copyToMap(Room source, Player* player, MapData* mapData);

#endif