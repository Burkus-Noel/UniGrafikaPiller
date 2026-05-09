#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h> // sqrt miatt kell
#include "world.h"
#include "graphics.h"
#include "player.h"

// Játék állapotok és textúrák
float minoX = 0, minoZ = 0;
float minoSpeed = 0.02f;
bool minoSpawned = false;
bool gameOver = false;

GLuint stoneTexture, ajtoTexture, fireTexture, floorTexture, torchTexture;

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("Hiba az SDL_image betoltesekor: %s\n", IMG_GetError());
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow("Minotaurusz Utvesztoje", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Alapvető GL beállítások
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    initLighting();
    initText();
    setupCameraProjection(45.0f, 800.0f/600.0f, 0.1f, 500.0f);
    
    // Textúrák beolvasása
    stoneTexture = loadTexture("stone.png");
    ajtoTexture  = loadTexture("ajto.jpg");
    floorTexture = loadTexture("floor.jpg");
    fireTexture  = loadTexture("tuz.png");
    torchTexture = loadTexture("torch.jpg");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    initFirstRoom();
    initParticlesEverywhere();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;

            if (event.type == SDL_MOUSEMOTION && !showMenu) {
                yaw   += event.motion.xrel * 0.1f;
                pitch += event.motion.yrel * 0.1f;

                if (pitch > 89.0f)  pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        handlePlayerMovement(state);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Kamera transzformációk
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(-posX, eyeHeight, -posZ); 

        setupFog(); 
        updateAndDrawParticles();
        drawMap();

        // Minotaurusz logika és mozgás
        if (minoSpawned && !gameOver) {
            drawMinotaur(minoX, minoZ);
            
            if (!showMenu) {
                float dirX = posX - minoX;
                float dirZ = posZ - minoZ;
                float distance = (float)sqrt(dirX * dirX + dirZ * dirZ);

                if (distance > 0.1f) {
                    minoX += (dirX / distance) * minoSpeed;
                    minoZ += (dirZ / distance) * minoSpeed;
                }

                if (distance < 0.6f) { 
                    gameOver = true;
                }
            }
        }

        drawUI(showMenu, gameOver);

        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // Fix ~60 FPS
    }

    // Takarítás
    IMG_Quit();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}