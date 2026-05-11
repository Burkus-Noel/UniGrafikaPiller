#include <SDL2/SDL_mixer.h>
#include <stdio.h>

Mix_Chunk *helmetSound = NULL;
Mix_Chunk *doorSound = NULL;
Mix_Chunk *endingMusic = NULL;
Mix_Chunk *winningMusic = NULL;

Mix_Music *backgroundMusic = NULL;
Mix_Music *bossMusic = NULL;

void initAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Hiba: %s\n", Mix_GetError());
    }
    
    helmetSound = Mix_LoadWAV("helmetup.mp3");
    doorSound = Mix_LoadWAV("door.mp3");
    endingMusic = Mix_LoadWAV("ending.mp3");
    winningMusic = Mix_LoadWAV("won.mp3");
    backgroundMusic = Mix_LoadMUS("maze.mp3");
    bossMusic = Mix_LoadMUS("bossbattle.mp3");
    Mix_PlayMusic(backgroundMusic, -1);
}

void playHelmetSound() {
    if (helmetSound != NULL) {
        Mix_PlayChannel(-1, helmetSound, 0);
    }
}

void playDoorSound() {
    if (doorSound != NULL) {
        Mix_PlayChannel(-1, doorSound, 0);
    }
}

void playMusic(int isBossLevel) {
    if(isBossLevel){
    Mix_HaltMusic();
    Mix_PlayMusic(bossMusic, -1);
    }
}

void endMusic() {
    Mix_HaltMusic();
    Mix_PlayChannel(-1, endingMusic, 0);
}

void wonMusic() {
    Mix_HaltMusic();
    Mix_PlayChannel(-1, winningMusic, 0);
}

void closeAudio() {
    if (helmetSound != NULL) {
        Mix_FreeChunk(helmetSound);
    }
    if (doorSound != NULL) {
        Mix_FreeChunk(doorSound);
    }
    if (backgroundMusic != NULL) {
        Mix_FreeMusic(backgroundMusic);
    }
    if (bossMusic != NULL) {
        Mix_FreeMusic(bossMusic);
    }
    if (endingMusic != NULL) {
        Mix_FreeChunk(endingMusic);
    }
    if (winningMusic != NULL) {
        Mix_FreeChunk(winningMusic);
    }
    Mix_CloseAudio();
}