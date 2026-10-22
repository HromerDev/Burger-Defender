#ifndef TOWER_LOGIC_H
#define TOWER_LOGIC_H
#include "../../dynamic_array.h"
#include "SDL2/SDL_image.h"
#include "../Enemy/enemy_logic.h"

typedef struct 
{    
    int type;
    int price;
    int location;   
    int damage;    
    double range;
    double turretAngle;
    double cooldownProgress;
    double cooldown;
    int towerLevel;
    SDL_Rect area;
    SDL_Texture *baseTexture;
    SDL_Texture *turretTexture;   
    int enemyTargetIndex;
    int id;
} Tower; 

typedef struct 
{
    double x;
    double y;
    int damage;
    int targetEnemyIndex;
    SDL_Rect area;
    SDL_Texture *projectileTexture;
    double velocity;
} Projectile;

extern int id;
extern dynarray *towers;
extern dynarray *projectiles;

void towerInit(SDL_Renderer *renderer);
void projectileAdd(int x, int y, int type, int targetIndex);
void projectileMove(double deltaTime);
void towerAdd(int type, int x, int y);
void processTowerShootCooldown(double deltaTime);
void towerShoot(int towerID);
void towerRemove(int x, int y);
void rotateTowers();
int returnClosestEnemy(int towerLocation, int range);
void renderProjectiles(SDL_Renderer *renderer);
void renderTowers(SDL_Renderer *renderer);
void towerFree();

#endif