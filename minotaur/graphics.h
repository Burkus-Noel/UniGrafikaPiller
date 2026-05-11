#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

//pi definíció
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Modell limitek
#define MAX_VERTICES 500000
#define MAX_FACES 1000000

// --- Erőforrások (Textúrák) ---
extern GLuint stoneTexture, fireTexture, ajtoTexture, floorTexture;
extern GLuint torchTexture, helmetTexture, swordTexture;

// --- Külső állapotok ---
extern float minoX, minoZ;
extern bool minoSpawned, helmetOn;

// --- Modell Struktúrák ---
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

// --- Rendszer inicializálás ---
void setupCameraProjection(float fovy, float aspect, float zNear, float zFar);
void initLighting();
void initText();
void setupFog();
void update_lighting();

// --- Részecskerendszer ---
void initParticlesEverywhere();
void updateAndDrawParticles();

// --- 3D Megjelenítés ---
void drawMap();
void drawWall(float x, float z, float h);
void drawDoor(float x, float z, float h, GLuint texture);
void drawFire(float x, float z, GLuint texture, GLuint frameTexture);
void drawMinotaur(float x, float z, SimpleModel* model);
void loadOBJ(const char* filename, SimpleModel* model);
void drawSimpleModel(SimpleModel* model);

// --- 2D UI és Szöveg ---
void drawUI(bool showMenu, bool gameOver, bool helmetOn, bool gameWon);
void renderText(const char* text, int x, int y, SDL_Color color);
void triggerBossMessage(const char* message, float duration);
void drawHelmetOverlay(GLuint helmetTexture);

// --- Segédfüggvények ---
GLuint loadTexture(const char* filename);

#endif