#ifndef GAMELOOP_LOGIC_H
#define GAMELOOP_LOGIC_H
#include "SDL2/SDL.h"

extern int health;
extern long coins;
extern long score;
extern int selectedTower;
extern int towerPrices[4];
extern int maxWaves;
extern int *waveData;
extern int waveDataIndex;
extern int currentWave;

void waveInit();
int spawnWaves(double deltaTime);
void updateWaveProgress(SDL_Renderer *renderer, int windowX, int windowY);
void selectTower(int x, int y);
void placeTower(int x, int y);
void removeTower(int x, int y);
void textUpdating(SDL_Renderer *renderer, int mouseX, int mouseY); 
void priceCheck(int type, int *price);
#endif