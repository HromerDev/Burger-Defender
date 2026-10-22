#include "enemy_logic.h"
#include "../Map/map_grid.h"
#include "../Map/map_reader.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../../dynamic_array.h"
#include "../Towers/tower_logic.h"
#include "../gameloop_logic.h"
#include <math.h>
SDL_Texture *enemyTextures[24];
dynarray *enemies;
void enemyInit(SDL_Renderer *renderer) 
{
    enemyTextures[0] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy0.png");
    enemyTextures[1] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy1.png");
    enemyTextures[2] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy2.png");
    enemyTextures[3] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy3.png");
    enemyTextures[4] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy4.png");
    enemyTextures[5] = IMG_LoadTexture(renderer, "Assets/Sprites/Enemies/enemy5.png");
    enemies = malloc(sizeof(dynarray));
    dynarray_init(enemies, 1); 
}
void enemyAdd(int type) 
{   
    Enemy *temp = malloc(sizeof(Enemy));

    returnTileRealLocationFloat(path[0], path[1], &temp->x, &temp->y);

    temp->lastPathX = path[0];
    temp->lastPathY = path[1];
    temp->targetPathX = path[2];
    temp->targetPathY = path[3];
    temp->turns = 0;

    switch(type) 
    {
        case 0: 
        {
            temp->health = 1;
            temp->velocity = 20;    
            temp->texture = enemyTextures[0];
            temp->score = 10;
            temp->reward = 1;
            break;
        }
        case 1: 
        {
            temp->health = 6;
            temp->velocity = 25;    
            temp->texture = enemyTextures[1];
            temp->score = 30;
            temp->reward = 2;
            break;
        }
        case 2: 
        {
            temp->health = 12;
            temp->velocity = 18;    
            temp->texture = enemyTextures[2];
            temp->score = 20;
            temp->reward = 8;
            break;
        }
        case 3: 
        {
            temp->health = 250;
            temp->velocity = 75;    
            temp->texture = enemyTextures[3];
            temp->score = 500;
            temp->reward = 150;
            break;
        }
        case 4: 
        {
            temp->health = 3000;
            temp->velocity = 150;    
            temp->texture = enemyTextures[4];
            temp->score = 1000;
            temp->reward = 600;
            break;
        }
        case 5: 
        {
            temp->health = 12500;
            temp->velocity = 50;    
            temp->texture = enemyTextures[5];
            temp->score = 50000;
            temp->reward = 10000000;
            break;
        }
    }

    dynarray_push(enemies, temp);
}
void drawEnemies(SDL_Renderer *renderer) 
{
    for(int i = 0; i < enemies->size; i++) 
    {
        Enemy *temp = enemies->items[i];
        SDL_Rect tempRect = {temp->x,temp->y, tileArea, tileArea};
        SDL_RenderCopy(renderer, temp->texture, NULL, &tempRect);
    }
}

void moveEnemies(double deltaTime) 
{   
    for(int i = 0; i < enemies->size; i++) 
    {       
        Enemy *temp = enemies->items[i];

        int x1;
        int y1;
        returnTileRealLocation(temp->lastPathX, temp->lastPathY, &x1, &y1);
        int x2;
        int y2;
        returnTileRealLocation(temp->targetPathX, temp->targetPathY, &x2, &y2);

        if(fabs(temp->x - x2) < 5 && fabs(temp->y - y2) < 5) 
        {
            temp->x = x2;
            temp->y = y2;

            temp->turns++;

            if(temp->turns >= pathSize - 1) 
            {
                temp->health = -1;
                health--;
                continue;
            }
                
            int pathIsActually2PointsAndIHateThatIDidntUseVectors = temp->turns * 2;

            temp->lastPathX = path[pathIsActually2PointsAndIHateThatIDidntUseVectors];
            temp->lastPathY = path[pathIsActually2PointsAndIHateThatIDidntUseVectors + 1];
            temp->targetPathX = path[pathIsActually2PointsAndIHateThatIDidntUseVectors + 2];
            temp->targetPathY = path[pathIsActually2PointsAndIHateThatIDidntUseVectors + 3];
            continue;
        }

        double speed = tileArea / temp->velocity * deltaTime * 20;
           
        if (temp->x < x2) 
            temp->x += speed;
        else if (temp->x > x2)
            temp->x -= speed;

        if (temp->y < y2)
            temp->y += speed;
        else if (temp->y > y2) 
            temp->y -= speed;
    }
}
void enemyKill() 
{
    for(int i = enemies->size - 1; i >= 0; i--) 
    {
        Enemy *temp = enemies->items[i];

        if(temp->health == 0) 
        {
            coins += temp->reward;
            score += temp->score;
        }

        if(temp->health <= 0) 
        {
            for(int j = 0; j < projectiles->size; j++)
            {
                Projectile *p = projectiles->items[j];
                if(p->targetEnemyIndex == i)
                    p->targetEnemyIndex = -1;
                else if(p->targetEnemyIndex > i)
                    p->targetEnemyIndex--;                                                      
            }
            dynarray_remove(enemies, temp);
        }            
    }    
}

void freeEnemies() 
{
    dynarray_free(enemies);
}