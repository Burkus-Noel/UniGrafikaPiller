#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "world.h"

typedef struct Engine Engine;

// "Konstruktor": Inicializálja az SDL-t, OpenGL-t és létrehozza az objektumot
Engine* Engine_Create(const char* title, int width, int height);

// Erőforrás kezelő: Betölti a textúrákat és modelleket
void Engine_LoadAssets(Engine* self);

// Eseménykezelő: Billentyűzet és egér
void Engine_HandleEvents(Engine* self);

// Logika: AI, ütközés, mozgás
void Engine_Update(Engine* self);

// Megjelenítés: Minden rajzolás itt történik
void Engine_Render(Engine* self);

// Állapot lekérdezés
bool Engine_IsRunning(Engine* self);

// "Destruktor": Felszabadítja a memóriát és leállítja az SDL-t
void Engine_Destroy(Engine* self);

#endif