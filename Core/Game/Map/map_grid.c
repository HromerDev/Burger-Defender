#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "map_grid.h"
#include "stdlib.h"
#include "../Towers/tower_logic.h"
#include "map_reader.h"
#include "../Enemy/enemy_logic.h"
#include "../gameloop_logic.h"
#include <stdio.h>
#include <math.h>

Tile *tileMap;
SDL_Texture *textures[24];
 
int gridSizeX;
int gridSizeY;

int tileArea;

int middleX;
int middleY;

void mapGridInit(int windowSizeX, int windowSizeY, SDL_Renderer *renderer) 
{   
    for(int i = 0; i < 8; i++) 
    {
        char temp[50] = "Assets/Sprites/Map/";
        char tempNumb[16];
        sprintf(tempNumb, "%d.png", i);
        strcat(temp, tempNumb);

        textures[i] = IMG_LoadTexture(renderer, temp);      
    }

    textures[19] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/buildNotPlacable.png");
    textures[20] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/buildPlacable.png");  
    textures[21] = IMG_LoadTexture(renderer, "Assets/Sprites/Map/portal.png");
    textures[22] = IMG_LoadTexture(renderer, "Assets/Sprites/Map/borgir.png");
    textures[23] = IMG_LoadTexture(renderer, "Assets/Sprites/Map/-1.png");

    mapInit("map", &gridSizeX, &gridSizeY);
    tileMap = malloc(gridSizeX * gridSizeY * sizeof(Tile));
    
    for (int i = 0; i < gridSizeX * gridSizeY; i++)
    {
        tileMap[i].tileType = textureID[i];

        if(i < gridSizeX && tileArea * gridSizeY == windowSizeY) 
        {
            tileMap[i].placedTowerID = -2;
            continue;
        }            

        tileMap[i].tileType = textureID[i];
        tileMap[i].placedTowerID = -1; // -1 == empty, -2 == path or ui
    }

    int offset = 0;
    int offsetter = 1;
    for(int i = 2; i < pathSize * 2 - 1; i += 2) 
    {
        if(path[i - 2] != path[i]) 
        {
            offset = 0;
            offsetter = (path[i - 2] > path[i]) ? -1 : 1;
                
            while(1)
            {
                int index = convertClosestTile1D(path[i - 2] + offset, path[i - 1]);
                tileMap[index].placedTowerID = -2;
                tileMap[index].tileType = 23;
                
                if(path[i - 2] + offset == path[i])
                    break;

                offset += offsetter;
            }
        }
        else if(path[i - 1] != path[i + 1]) 
        {
            offset = 0;
            offsetter = (path[i - 1] > path[i + 1]) ? -1 : 1;

            while(1)
            {
                int index = convertClosestTile1D(path[i - 2], path[i - 1] + offset);
                tileMap[index].placedTowerID = -2;
                tileMap[index].tileType = 23;


                if(path[i - 1] + offset == path[i + 1])
                    break;

                offset += offsetter;
            }
        }            
    }
    
    int tileWidth = windowSizeX / gridSizeX;
    int tileHeight = windowSizeY / gridSizeY;
    tileArea = tileWidth < tileHeight ? tileWidth : tileHeight;
    
    middleX = (windowSizeX / 2) - (tileArea * gridSizeX) / 2;
    middleY = (windowSizeY / 2) - (tileArea * gridSizeY) / 2;
    enemyAdd(0);
}

void drawGrid(SDL_Renderer *renderer, int mouseX, int mouseY) 
{   int x;
    int y;
    double d;
    int offsetY = 0;
    for(int i = 0;i < gridSizeX * gridSizeY; i++) 
    {         
        if(i > 0 && i % gridSizeX == 0)
            offsetY++;
         
        SDL_Rect temp = {( i % gridSizeX) * tileArea + middleX, offsetY * tileArea + middleY, tileArea, tileArea};
        int x;
        int y;
        
        SDL_RenderCopy(renderer, textures[tileMap[i].tileType], NULL, &temp); 

        if(convertClosestTile1D(path[0], path[1]) == i && (currentWave != maxWaves)) 
        {
            SDL_RenderCopy(renderer, textures[21], NULL, &temp);
            continue;
        }
        else if(convertClosestTile1D(path[pathSize * 2 - 2], path[pathSize * 2 - 3])  == i) 
        {
            SDL_RenderCopy(renderer, textures[22], NULL, &temp);
            continue;
        }

        if (returnClosestTile1D(mouseX, mouseY, NULL) == i) 
        {
            if(selectedTower == -2)
                continue;
            
            if(selectedTower >= 0) 
            {
                if(tileMap[i].placedTowerID == -1)
                    SDL_RenderCopy(renderer, textures[20], NULL, &temp);
                else
                    SDL_RenderCopy(renderer, textures[19], NULL, &temp);
            }
        }
    }

    renderTowers(renderer);
}

int returnClosestTile1D(int x, int y, double *distance) 
{   
        int offsetY = 0;
        int closestTileIndex = -1;
        double closestDistance = INT32_MAX;
        for(int i = 0; i < gridSizeX * gridSizeY; i++) 
        {
            if(i > 0 && i % gridSizeX == 0)
            offsetY++;
            
            double currentSqrt = sqrt(pow(((i % gridSizeX) * tileArea + middleX - x) + tileArea / 2, 2) + pow(offsetY * tileArea + middleY - y + tileArea / 2, 2));
            
            if(currentSqrt < closestDistance && currentSqrt < tileArea) 
            {
                closestDistance = currentSqrt;
                closestTileIndex = i;
            }
        }    
        if(distance != NULL)
            *distance = closestDistance; 
        return closestTileIndex;                          
}

void returnTileRealLocation(int tileX, int tileY, int *x, int *y) 
{
    *x = tileX * tileArea + middleX;
    *y = tileY * tileArea + middleY;
}

void returnTileRealLocationFloat(int tileX, int tileY, double *x, double *y)
{
    *x = tileX * tileArea + middleX;
    *y = tileY * tileArea + middleY;
}

int convertClosestTile1D(int x, int y)
{
    return y * gridSizeX + x;
}

void convertClosestTile2D(int index, int *x, int *y) 
{
    *x = index % gridSizeX;     
    *y = index / gridSizeX; 
}

void returnAllTileLocationData(int positionX, int positionY, int *index, int *xPos, int *yPos) 
{
    *index = returnClosestTile1D(positionX, positionY, NULL);
    convertClosestTile2D(*index, xPos, yPos);
}

void freeGrid() 
{
    free(tileMap);
    freeMapData();
}