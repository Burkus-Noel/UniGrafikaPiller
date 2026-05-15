#define _USE_MATH_DEFINES
#include "graphics.h"
#include "world.h"
#include "draw.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>

TTF_Font* font = NULL; 
extern int swordCount;

void initText() {
    if (TTF_Init() == -1) return;
    font = TTF_OpenFont("assets/font.ttf", 20);
}

void renderText(const char* text, int x, int y, SDL_Color color) {
    if (!font || !text) return;
    SDL_Surface* surf = TTF_RenderUTF8_Solid(font, text, color);
    if (!surf) return;
    SDL_Surface* rgbaSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!rgbaSurf) return;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaSurf->w, rgbaSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub(color.r, color.g, color.b, color.a); 

    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(x, y);
        glTexCoord2f(1,0); glVertex2f(x + rgbaSurf->w, y);
        glTexCoord2f(1,1); glVertex2f(x + rgbaSurf->w, y + rgbaSurf->h);
        glTexCoord2f(0,1); glVertex2f(x, y + rgbaSurf->h);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glDeleteTextures(1, &tex);
    SDL_FreeSurface(rgbaSurf);
}


void drawHelmetOverlay(GLuint texture) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); 
        glVertex2f(-50.0f, -200.0f);   
        glTexCoord2f(1.0f, 0.0f); 
        glVertex2f(850.0f, -200.0f); 
        glTexCoord2f(1.0f, 1.0f); 
        glVertex2f(850.0f, 600.0f);  

        // Bal alsó
        glTexCoord2f(0.0f, 1.0f); 
        glVertex2f(-50.0f, 600.0f);   
    glEnd();
    
    glDisable(GL_BLEND);
}

void drawUI(GameState* game, Assets* assets) {
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    SDL_Color white = {255, 255, 255, 255}, yellow = {255, 255, 0, 255}, green = {0, 255, 0, 255}, red = {255, 0, 0, 255};
    
    if (game->helmetOn && !game->gameOver && !game->showMenu) {
        drawHelmetOverlay(assets->helmet);
    }

    if (!game->gameOver && !game->gameWon) {
        renderText("Controls: F1", 50, 550, green);
    }

    if (game->showMenu) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_TEXTURE_2D); 
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f); 
        glBegin(GL_QUADS);
            glVertex2f(50, 50); glVertex2f(750, 50);
            glVertex2f(750, 550); glVertex2f(50, 550);
        glEnd();
        
        glEnable(GL_TEXTURE_2D);
        renderText("--- CONTROLS ---", 280, 80, yellow);
        renderText("W/S/A/D - Movement", 100, 160, white);
        renderText("SHIFT   - Run", 100, 200, white);
        renderText("C       - Crouch", 100, 240, white);
        renderText("+ / -   - Brightness", 100, 280, white);
        renderText("V       - Visor", 100, 320, white);
        renderText("F1      - Resume", 320, 510, yellow);
    }


    if (!game->gameOver && !game->showMenu) {
        char swordText[64];
        SDL_Color textColor;

        if (game->swordCount < 10) {
            sprintf(swordText, "SWORDS: %d / 10", game->swordCount);
            textColor.r = 255; textColor.g = 255; textColor.b = 255; textColor.a = 255;
        } else {
            sprintf(swordText, "SWORD REFORGED");
            float pulse = 0.75f + 0.25f * sinf(SDL_GetTicks() * 0.005f);
            
            textColor.r = 0; 
            textColor.g = (Uint8)(255.0f * pulse); 
            textColor.b = (Uint8)(255.0f * pulse);
            textColor.a = 255;
        }
        renderText(swordText, 50, 520, textColor);
    }

    if (game->gameOver) {
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(0, 0); glVertex2f(800, 0);
            glVertex2f(800, 600); glVertex2f(0, 600);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
        
        renderText("GAME OVER", 330, 280, red);
        renderText("Next time try to bring a sword to a bull fight.", 220, 330, white);
    }
    if (game->gameWon) {
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(0, 0); glVertex2f(800, 0);
            glVertex2f(800, 600); glVertex2f(0, 600);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
        
        renderText("YOU'RE WINNER!", 330, 280, green);
        renderText("Congratulations on defeating the Minotaur!", 220, 330, white);
    }

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
}