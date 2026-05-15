#include "engine.h"
#include "graphics.h"
#include "player.h"
#include "sound.h"
#include "draw.h"
#include "UI.h"
#include <stdio.h>
#include <time.h>

struct Engine {
    SDL_Window* window;
    SDL_GLContext glContext;
    bool running;
    int width;
    int height;
    GameState state;
    Assets assets;
};

GameState game;
Assets assets;

Engine* Engine_Create(const char* title, int width, int height) {
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return NULL;
    
    Engine* self = malloc(sizeof(Engine));
    if (!self) return NULL;

    self->width = width;
    self->height = height;
    self->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                   width, height, SDL_WINDOW_OPENGL);
    
    if (!self->window) {
        free(self);
        return NULL;
    }

    self->glContext = SDL_GL_CreateContext(self->window);
    self->running = true;

    self->state = (GameState){true, false, false, 0, 5.0f, 5.0f, 0.2f, false, false, false};

    // Alaprendszerek indítása
    initAudio();
    initLighting();
    initText();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    setupCameraProjection(45.0f, (float)width/height, 0.1f, 500.0f);

    return self;
}

void Engine_LoadAssets(Engine* self) {
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("SDL_image hiba: %s\n", IMG_GetError());
    }

    self->assets.stone  = loadTexture("assets/stone.png");
    self->assets.ajto   = loadTexture("assets/ajto.jpg");
    self->assets.floor  = loadTexture("assets/floor.jpg");
    self->assets.fire   = loadTexture("assets/tuz.png");
    self->assets.torch  = loadTexture("assets/torch.jpg");
    self->assets.helmet = loadTexture("assets/visor3.png");
    self->assets.sword  = loadTexture("assets/sword.png");
    
    loadOBJ("assets/minotaur.obj", &self->assets.minoModel);
    self->assets.minoModel.texture = loadTexture("assets/minotaur.png");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    initFirstRoom();
    initParticlesEverywhere();
}

void Engine_HandleEvents(Engine* self) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) self->running = false;
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) self->running = false;
            if (event.key.keysym.sym == SDLK_F1) {
                self->state.showMenu = !self->state.showMenu;
                SDL_SetRelativeMouseMode(self->state.showMenu ? SDL_FALSE : SDL_TRUE);
            }
            if (event.key.keysym.sym == SDLK_v) self->state.helmetOn = !self->state.helmetOn;
        }

        if (event.type == SDL_MOUSEMOTION && !self->state.showMenu && !self->state.gameOver) {
            // Itt a globális yaw/pitch-et használjuk (vagy átrakhatod a GameState-be)
            extern float yaw, pitch; 
            yaw   += event.motion.xrel * 0.1f;
            pitch += event.motion.yrel * 0.1f;
            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
    }
}

void Engine_Update(Engine* self) {
    if (self->state.gameOver || self->state.gameWon) return;

    handlePlayerMovement(SDL_GetKeyboardState(NULL));
    checkSwordPickup(&self->state);

    // Minotaurusz AI
    if (self->state.minoSpawned && !self->state.showMenu) {
        float dirX = posX - self->state.minoX;
        float dirZ = posZ - self->state.minoZ;
        float distance = sqrtf(dirX * dirX + dirZ * dirZ);

        if (distance > 0.1f) {
            self->state.minoX += (dirX / distance) * self->state.minoSpeed;
            self->state.minoZ += (dirZ / distance) * self->state.minoSpeed;
        }

        if (distance < 0.6f) {
            if (self->state.swordCount >= 10) {
                self->state.gameWon = true;
                wonMusic();
            } else {
                self->state.gameOver = true;
                endMusic();
            }
        }
    }
}

void Engine_Render(Engine* self) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera transzformáció
    extern float posX, posZ, yaw, pitch, eyeHeight;
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(-posX, eyeHeight, -posZ); 

    setupFog(); 
    updateAndDrawParticles();
    drawMap(&self->state, &self->assets);

    if (self->state.minoSpawned) {
        drawMinotaur(self->state.minoX, self->state.minoZ, &self->assets.minoModel);
    }

    // UI réteg
    drawUI(&self->state, &self->assets);

    SDL_GL_SwapWindow(self->window);
}

bool Engine_IsRunning(Engine* self) {
    return self->running;
}

void Engine_Destroy(Engine* self) {
    if (!self) return;
    IMG_Quit();
    SDL_GL_DeleteContext(self->glContext);
    SDL_DestroyWindow(self->window);
    closeAudio();
    SDL_Quit();
    free(self);
}