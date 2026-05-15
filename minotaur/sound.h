#ifndef SOUND_H
#define SOUND_H

// hangok lejátszása
void initAudio();
void playHelmetSound();
void playDoorSound();
void playMusic(int isBossLevel);
void endMusic();
void wonMusic();
void closeAudio();

#endif