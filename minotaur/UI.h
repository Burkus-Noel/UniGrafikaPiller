// --- 2D UI ---
void drawUI(GameState* game, Assets* assets);
void triggerBossMessage(const char* message, float duration);
void drawHelmetOverlay(GLuint helmetTexture);
// --- Szöveg renderelés ---
void renderText(const char* text, int x, int y, SDL_Color color);
void initText();