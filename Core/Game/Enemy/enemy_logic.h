#ifndef ENEMY_LOGIC_H
#define ENEMY_LOGIC_H
#include <SDL2/SDL_image.h>
#include "../../dynamic_array.h"

typedef struct 
{
    int enemyType;
    long score;
    int reward;
    double x;
    double y;
    int health;
    int lastPathX;
    int lastPathY;
    int targetPathX;
    int targetPathY;
    int turns;
    double velocity;
    SDL_Texture *texture;
} Enemy;

extern dynarray *enemies;

void enemyInit(SDL_Renderer *renderer);
void enemyAdd(int type);
void drawEnemies(SDL_Renderer *renderer);
void moveEnemies(double deltaTime);
void enemyKill();
void freeEnemies();

#endif