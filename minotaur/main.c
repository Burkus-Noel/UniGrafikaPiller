#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "world.h"
#include "graphics.h"
#include "player.h"
#include "sound.h"

// Globális állapotok
float minoX = 0, minoZ = 0;
float minoSpeed = 0.02f;
bool minoSpawned = false;
bool gameOver = false;
bool gameWon = false;
int swordCount = 0;

// Külső változók és segédfüggvények
extern float bossTextTimer;
extern bool bossTextTriggered;
void triggerBossMessage(const char* message, float duration);

// Textúra azonosítók
GLuint stoneTexture, ajtoTexture, fireTexture, floorTexture, torchTexture, helmetTexture, swordTexture;
SimpleModel minoModel;

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    
    // Alaprendszerek inicializálása
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    initAudio();
    
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("SDL_image hiba: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Minotaurusz Utvesztoje", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    initLighting();
    initText();
    setupCameraProjection(45.0f, 800.0f/600.0f, 0.1f, 500.0f);
    
    stoneTexture  = loadTexture("assets/stone.png");
    ajtoTexture   = loadTexture("assets/ajto.jpg");
    floorTexture  = loadTexture("assets/floor.jpg");
    fireTexture   = loadTexture("assets/tuz.png");
    torchTexture  = loadTexture("assets/torch.jpg");
    helmetTexture = loadTexture("assets/visor3.png");
    swordTexture  = loadTexture("assets/sword.png");
    
    loadOBJ("assets/minotaur.obj", &minoModel);
    minoModel.texture = loadTexture("assets/minotaur.png");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    initFirstRoom();
    initParticlesEverywhere();

    bool running = true;
    SDL_Event event;

    // Fő játékciklus
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;

            if (event.type == SDL_MOUSEMOTION && !showMenu && !gameOver && !gameWon) {
                yaw   += event.motion.xrel * 0.1f;
                pitch += event.motion.yrel * 0.1f;
                if (pitch > 89.0f)  pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;
            }
        }

        if (!gameOver && !gameWon) {
            handlePlayerMovement(SDL_GetKeyboardState(NULL));
        }

        // Renderelés
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(-posX, eyeHeight, -posZ); 
        checkSwordPickup();
        setupFog(); 
        updateAndDrawParticles();
        drawMap();

        // Időzítők frissítése
        if (bossTextTimer > 0 && !showMenu) bossTextTimer -= 0.016f;

        // Minotaurusz logika
        if (minoSpawned && !gameOver && !gameWon) {
            drawMinotaur(minoX, minoZ, &minoModel); 
            
            if (!showMenu) {
                if (!bossTextTriggered) triggerBossMessage("FRESH MEAT!", 3.0f);

                float dirX = posX - minoX;
                float dirZ = posZ - minoZ;
                float distance = sqrtf(dirX * dirX + dirZ * dirZ);

                if (distance > 0.1f) {
                    minoX += (dirX / distance) * minoSpeed;
                    minoZ += (dirZ / distance) * minoSpeed;
                }

                if (distance < 0.6f) {
                    if (swordCount >= 10) {
                        gameWon = true;
                        wonMusic();
                    } else {
                        gameOver = true;
                        endMusic(); 
                    }
                }
            }
        }

        drawUI(showMenu, gameOver, helmetOn, gameWon);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16); 
    }

    IMG_Quit();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    closeAudio();
    SDL_Quit();
    return 0;
}