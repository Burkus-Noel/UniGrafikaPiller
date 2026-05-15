#define _USE_MATH_DEFINES
#include "player.h"
#include "world.h"
#include "sound.h"
#include <math.h>
#include <SDL2/SDL.h>

// Állapotváltozók
bool showMenu = false;
bool helmetOn = false;
bool f1Pressed = false;

float posX = 0.0f, posZ = 0.0f;
float yaw = 0.0f, pitch = 0.0f;
float eyeHeight = -1.0f;
static uint32_t lastVPress = 0;

extern int map[MAX_MAP_SIZE][MAX_MAP_SIZE]; 
extern int currentWidth, currentHeight;
extern float brightness;

void checkSwordPickup(GameState* state) {
    int gridX = (int)((posX + 1.0f) / 2.0f);
    int gridZ = (int)((posZ + 1.0f) / 2.0f);

    if (gridX >= 0 && gridX < currentWidth && gridZ >= 0 && gridZ < currentHeight) {
        if (map[gridZ][gridX] == 6) {
            map[gridZ][gridX] = 0;
            state->swordCount++;
        }
    }
}

// Ütközésvizsgálat
int isWall(float x, float z) {
    int i = (int)((x + 1.0f) / 2.0f);
    int j = (int)((z + 1.0f) / 2.0f);

    if (i < 0 || i >= currentWidth || j < 0 || j >= currentHeight) return 1;

    int cell = map[j][i];
    return (cell == 1 || cell == 4);
}

void handlePlayerMovement(const Uint8* state) {
    if (state[SDL_SCANCODE_F1]) {
        if (!f1Pressed) {
            game.showMenu = !game.showMenu;
            f1Pressed = true;
        }
    } else {
        f1Pressed = false;
    }

    float speed = (state[SDL_SCANCODE_LSHIFT]) ? 0.12f : 0.06f;
    
    if (state[SDL_SCANCODE_C]) {
        if (eyeHeight < -0.4f) eyeHeight += 0.08f; 
    } else {
        if (eyeHeight > -1.0f) eyeHeight -= 0.08f;
    }

    uint32_t currentTime = SDL_GetTicks();
    if (state[SDL_SCANCODE_V] && currentTime > lastVPress + 300) {
        game.helmetOn = !game.helmetOn;
        playHelmetSound();
        lastVPress = currentTime;
    }

    float rad = yaw * (float)M_PI / 180.0f;
    float fx = sinf(rad), fz = -cosf(rad);
    float rx = cosf(rad), rz = sinf(rad);

    float moveX = 0, moveZ = 0;
    if (state[SDL_SCANCODE_W]) { moveX += fx; moveZ += fz; }
    if (state[SDL_SCANCODE_S]) { moveX -= fx; moveZ -= fz; }
    if (state[SDL_SCANCODE_D]) { moveX += rx; moveZ += rz; }
    if (state[SDL_SCANCODE_A]) { moveX -= rx; moveZ -= rz; }

    //ütközés vizsgálat
    if (moveX != 0 || moveZ != 0) {
        float nextX = posX + moveX * speed;
        float nextZ = posZ + moveZ * speed;
        float off = 0.3f; 

        if (!isWall(nextX + (moveX > 0 ? off : -off), posZ)) posX = nextX;
        if (!isWall(posX, nextZ + (moveZ > 0 ? off : -off))) posZ = nextZ;
    }

    // Ajtón való áthaladás
    int curI = (int)((posX + 1.0f) / 2.0f);
    int curJ = (int)((posZ + 1.0f) / 2.0f);

    if (map[curJ][curI] == 3) {
        progressToNextLevel();
        playDoorSound();
    }

    // Fényerő állítás
    if (state[SDL_SCANCODE_KP_PLUS]  && brightness < 5.0f)  brightness += 0.05f;
    if (state[SDL_SCANCODE_KP_MINUS] && brightness > -0.5f) brightness -= 0.05f;
}