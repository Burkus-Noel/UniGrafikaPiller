#include <SDL2/SDL.h>
#include <stdio.h>
#include "engine.h" // Ez az új "osztályunk" fejléce

int main(int argc, char* argv[]) {
    // 1. Objektum létrehozása (Konstruktor hívás)
    Engine* engine = Engine_Create("Minotaurusz Utvesztoje", 800, 600);
    if (!engine) {
        fprintf(stderr, "Hiba az Engine inicializálásakor!\n");
        return 1;
    }

    // 2. Erőforrások betöltése (Adatfeltöltés)
    Engine_LoadAssets(engine);

    // 3. Fő ciklus (Üzenetkezelés és futtatás elrejtve)
    while (Engine_IsRunning(engine)) {
        Engine_HandleEvents(engine);
        Engine_Update(engine);
        Engine_Render(engine);
    }

    // 4. Takarítás (Destruktor hívás)
    Engine_Destroy(engine);

    return 0;
}