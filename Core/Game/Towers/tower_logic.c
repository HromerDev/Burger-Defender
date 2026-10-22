#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tower_logic.h"
#include "../Map/map_grid.h"
#include "../../dynamic_array.h"
#include "../Enemy/enemy_logic.h"
#include <math.h>
#include "../gameloop_logic.h"

int id = 0;
dynarray *towers;
dynarray *projectiles;
SDL_Texture *towerTextures[24];
SDL_Texture *projectileTextures[2];

void towerInit(SDL_Renderer *renderer) 
{
    towerTextures[0] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/base0.png");
    towerTextures[1] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/turret00.png");
    towerTextures[2] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/turret10.png");
    towerTextures[3] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/turret20.png");
    towerTextures[4] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/turret30.png");
    projectileTextures[0] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/projectile0.png");
    projectileTextures[1] = IMG_LoadTexture(renderer, "Assets/Sprites/Turrets/projectile1.png");
    towers = malloc(sizeof(dynarray));
    projectiles = malloc(sizeof(dynarray));

    dynarray_init(towers, 1);
    dynarray_init(projectiles, 1);
}
void projectileAdd(int x, int y, int type, int targetIndex) 
{
    Projectile *temp = malloc(sizeof(Projectile));
    
    temp->x = (double)x + tileArea / 2;
    temp->y = (double)y + tileArea / 2;
    
    temp->area = (SDL_Rect) {middleX + x * tileArea, middleY + y * tileArea, tileArea / 4, tileArea / 4};

    temp->x-= temp->area.h / 2;
    temp->y-= temp->area.h / 2;
    temp->targetEnemyIndex = targetIndex;
    
    switch(type) 
    {
        case 0:
            temp->damage = 1;
            temp->velocity = 500;
            temp->projectileTexture = projectileTextures[0];
        break;
        case 1:
            temp->damage = 8;
            temp->velocity = 1000;
            temp->projectileTexture = projectileTextures[1];
        break;
        case 2:
            temp->damage = 3;
            temp->velocity = 2000;
            temp->projectileTexture = projectileTextures[0];
        break;
        case 3:
            temp->damage = 150;
            temp->velocity = 500;
            temp->projectileTexture = projectileTextures[1];
        break;
    }
 
    dynarray_push(projectiles, temp);
}

void projectileMove(double deltaTime) 
{
    for(int i = projectiles->size - 1; i >= 0; i--) 
    {
        Projectile *temp = projectiles->items[i];

        int targetValid = 0;
        if(temp->targetEnemyIndex >= 0 && temp->targetEnemyIndex < enemies->size) 
        {
            Enemy *enemy = enemies->items[temp->targetEnemyIndex];
            if(enemy->health > 0)
                targetValid = 1;
        }

        if(targetValid == 0) 
        {
            int newTarget = -1;
            for(int j = 0; j < enemies->size; j++) 
            {
                Enemy *potencialEnemy = enemies->items[j];
                double directionX = (potencialEnemy->x + tileArea / 2) - temp->x;
                double directionY = (potencialEnemy->y + tileArea / 2) - temp->y;
                double distance = sqrt(pow(directionX, 2) + pow(directionY, 2));

                if(distance <= tileArea * 5) 
                {
                    newTarget = j;
                    break;
                }
            }

            if(newTarget == -1) 
            {
                dynarray_remove(projectiles, temp);
                continue;
            }
            temp->targetEnemyIndex = newTarget;
        }
            
        Enemy *tempEnemy = enemies->items[temp->targetEnemyIndex];

        double directionX = (tempEnemy->x + tileArea / 2) - temp->x;
        double directionY = (tempEnemy->y + tileArea / 2) - temp->y;

        double distance = sqrt(pow(directionX,2) + pow(directionY,2));

        if(tempEnemy->health <= 0) 
        {
            dynarray_remove(projectiles, temp);
            continue;
        }

        if(distance < tileArea / 4 && tempEnemy->health > 0) 
        {
            tempEnemy->health-= temp->damage;   
            if(tempEnemy->health < 0)
                tempEnemy->health = 0;
                
            dynarray_remove(projectiles, temp);            
            continue;
        }
            
        double speed = temp->velocity;
        temp->x += (directionX / distance) * speed * deltaTime;
        temp->y += (directionY / distance) * speed * deltaTime;

        temp->area.x = (int)temp->x;
        temp->area.y = (int)temp->y;
    }
}

void towerAdd(int type, int x, int y) 
{
    int tempIndex = returnClosestTile1D(x, y, NULL);
    if(tempIndex == -1)
        return;

    if(tileMap[tempIndex].placedTowerID != -1) 
        return;
    
    Tower *temp = malloc(sizeof(Tower));

    temp->location = tempIndex;
    temp->type = type;
    convertClosestTile2D(tempIndex, &x, &y);
    temp->area = (SDL_Rect) {middleX + x * tileArea, middleY + y * tileArea, tileArea, tileArea};
    temp->enemyTargetIndex = -1;
    temp->turretAngle = 0;
    tileMap[temp->location].placedTowerID = id;
    temp->id = id;
    temp->towerLevel = 1;
    temp->cooldownProgress = 0;
    id++;
    
    switch(type) 
    {
        case 0: 
        {          
            temp->cooldown = 2;    
            temp->price = towerPrices[0];
            temp->baseTexture = towerTextures[0];
            temp->turretTexture = towerTextures[1];
            temp->range = 3 * tileArea;
        }
        break;
        case 1:
        {          
            temp->cooldown = 3;
            temp->price = towerPrices[1];    
            temp->baseTexture = towerTextures[0];
            temp->turretTexture = towerTextures[2];
            temp->range = 4 * tileArea;
        }
        break;
        case 2:
            temp->cooldown = 0.35;
            temp->price = towerPrices[2];
            temp->baseTexture = towerTextures[0];
            temp->turretTexture = towerTextures[3];
            temp->range = 1.5 * tileArea;
        break;
        case 3:
            temp->cooldown = 15;
            temp->price = towerPrices[3];
            temp->baseTexture = towerTextures[0];
            temp->turretTexture = towerTextures[4];
            temp->range = 30 * tileArea;
        break;
    }

    coins -= temp->price;
    dynarray_push(towers, temp);    
}
void processTowerShootCooldown(double deltaTime) 
{
    for(int i = 0; i < towers->size; i++) 
    {
        Tower *temp = towers->items[i];

        if(temp->cooldownProgress > temp->cooldown) 
        {
            towerShoot(temp->id);
            temp->cooldownProgress -= temp->cooldown;
            continue;
        }
        temp->cooldownProgress += deltaTime;
    }
}
void towerShoot(int towerID) 
{
    int towerIndex = -1;
    for(int i = 0; i < towers->size; i++) 
    {
        Tower *temp = towers->items[i];

        if(temp->id == towerID) 
        {
            towerIndex = i;
            break;
        }
    }
    if (towerIndex == -1)
        return;
        
    Tower *temp = towers->items[towerIndex];
    
    if (temp->enemyTargetIndex < 0 || temp->enemyTargetIndex >= enemies->size)        
        return;

    int projectileX;
    int projectileY;
    convertClosestTile2D(temp->location, &projectileX, &projectileY);
    returnTileRealLocation(projectileX, projectileY, &projectileX, &projectileY);

    Enemy *tempEnemy = enemies->items[temp->enemyTargetIndex];

    projectileAdd((double)projectileX,(double)projectileY, temp->type, temp->enemyTargetIndex);
}

void towerRemove(int x, int y) 
{    
    int currentTile = returnClosestTile1D(x, y, NULL);

    if(currentTile == -1)
        return;

    for(int i = towers->size - 1; i >= 0; i--)  
    {
        Tower *temp = towers->items[i];
 
        if(currentTile == temp->location) 
        {
            coins += temp->price / 2 + 1;
            dynarray_remove(towers, temp);            
        }      
    }
    
    tileMap[currentTile].placedTowerID = -1;
    
}
void rotateTowers()
{   
    if(enemies->size < 1)
        return;
        
    Enemy *currentEnemy;
    for(int i = 0; i < towers->size; i++)
    {   
        Tower *temp = towers->items[i];
        
        int enemyIndex;
        temp->enemyTargetIndex = returnClosestEnemy(temp->location, temp->range);
        
        if(temp->enemyTargetIndex == -1)
            continue;

        if(temp->type == 2)
            continue;

        int tileX;
        int tileY;
        convertClosestTile2D(temp->location, &tileX, &tileY);

        double towerX = middleX + tileX * tileArea + tileArea / 2;
        double towerY = middleY + tileY * tileArea + tileArea / 2;
        
        currentEnemy = enemies->items[temp->enemyTargetIndex];

        double directionX = currentEnemy->x + tileArea / 2 - towerX;
        double directionY = currentEnemy->y + tileArea / 2 - towerY;
        temp->turretAngle = (atan2(directionY, directionX) * 180 / 3.141592654) + 180;
    }
}
int returnClosestEnemy(int towerLocation, int range) 
{   
    int x;
    int y;
    convertClosestTile2D(towerLocation, &x, &y);
    returnTileRealLocation(x, y, &x, &y);

    int closestEnemyIndex = -1;
    double closestDistance = INT32_MAX;

    for(int i = 0; i < enemies->size; i++) 
    {         
        Enemy *temp = enemies->items[i];

        double currentSqrt = sqrt(pow(temp->x - x, 2) + pow(temp->y - y, 2));
            
        if(currentSqrt < range)         
            return i;        
    }

    return closestEnemyIndex;                       
}

void renderProjectiles(SDL_Renderer *renderer) 
{
    for(int i = 0; i < projectiles->size; i++) 
    {
        Projectile *temp = projectiles->items[i]; 
        SDL_RenderCopy(renderer, temp->projectileTexture, NULL, &temp->area);
    }    
} 

void renderTowers(SDL_Renderer *renderer)
{
    for(int i = 0; i < towers->size; i++) 
    {
        Tower *temp = towers->items[i]; 
        SDL_RenderCopy(renderer, temp->baseTexture, NULL, &temp->area);
        SDL_RenderCopyEx(renderer, temp->turretTexture, NULL, &temp->area, temp->turretAngle, NULL, 0);
    }    
}

void towerFree() 
{
    //dynarray_free(towers); už je prý free???
}