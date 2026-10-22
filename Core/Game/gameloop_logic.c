#include "gameloop_logic.h"
#include "../UI/user_interface.h"
#include "Map/map_grid.h"
#include "Towers/tower_logic.h"
#include "SDL2/SDL.h"
#include "string.h"
#include "stdio.h"

int health = 30;
long coins = 25;
long score = 0;
int selectedTower = -2;
char tempText[32];
int towerPrices[4] = {10, 65, 250, 450};
int currentWave = 1;
int maxWaves = 0;
int *waveData = NULL;
char fileLine[200];
int waveDataIndex = 0;
int alreadySpawned = 0;
double spawnTimer = 0;
double baseSpawnInterval = 0.5;
double currentSpawnInterval = 1;
int waveInProgress = 0;

void waveInit() 
{
    FILE *waveFile = fopen("Maps/Waves.mapData", "r");

    if(maxWaves == 0) 
    {
        fgets(fileLine, sizeof(fileLine), waveFile); 
        maxWaves = atoi(fileLine);
    }

    if(waveData != NULL) 
    {
        free(waveData);
        waveData = NULL;
    }
    
    waveData = malloc(sizeof(int) * 200);
    waveDataIndex = 0;
    alreadySpawned = 0;
    spawnTimer = 0;

    for(int i = 1; i <= maxWaves; i++) 
    {
        fgets(fileLine, sizeof(fileLine), waveFile);        
        if(i == currentWave) 
        {
            char *temp = fileLine;

            while(*temp != '\0')
            {
                while(*temp == ' ' || *temp == '\n') temp++;
                if(*temp == '\0') break;

                waveData[waveDataIndex++] = atoi(temp);

                while(*temp != ' ' && *temp != '\n' && *temp != '\0') temp++;
            }
            break;
        }
    }

    fclose(waveFile);
    waveInProgress = 1; 
}
int spawnWaves(double deltaTime) 
{
    if(alreadySpawned >= waveDataIndex)
        return 0;
  
    spawnTimer += deltaTime;

    if(spawnTimer > baseSpawnInterval) 
    {
        spawnTimer -= baseSpawnInterval;
        enemyAdd(waveData[alreadySpawned]);
        alreadySpawned++;
    }
      
    return 1;
}

void updateWaveProgress(SDL_Renderer *renderer, int windowX, int windowY)
{
    if(alreadySpawned >= waveDataIndex && enemies->size == 0)
    {
        waveInProgress = 0;
        currentWave++;

        if(currentWave <= maxWaves)        
            waveInit();
        
        else
        {
            currentScene = 2;
            uiEndScreenInit(renderer, windowX, windowY);                      
        }
    }
}

void selectTower(int x, int y) 
{
    int index = buttonIndex(x,y, ingameUIElements, ingameUIElementAmount);

    if(11 == index) 
    {
        selectedTower = -1;
        return;
    }

    if(index >= 7 && index < 11) 
    {
        selectedTower = 10 - index;
           
        if(selectedTower < 0 || selectedTower >= 4 || towerPrices[selectedTower] > coins)
            selectedTower = -2;
                    
        return;
    }
    selectedTower = -2;
}

void textUpdating(SDL_Renderer *renderer, int mouseX, int mouseY) 
{
    sprintf(tempText,"%ld",coins);
    updateText(renderer, &ingameUIElements[2], tempText);
    sprintf(tempText,"%d",health);
    updateText(renderer, &ingameUIElements[4], tempText);
    sprintf(tempText,"%ld",score);
    updateText(renderer, &ingameUIElements[6], tempText);

    int index = buttonIndex(mouseX, mouseY, ingameUIElements, ingameUIElementAmount);

    if(index >= 7 && index < 11) 
    {
        int tempSelectedTower = 10 - index;

        if(tempSelectedTower >= 0 && tempSelectedTower < 4)
        {
            sprintf(tempText, "Cost: %dC", towerPrices[tempSelectedTower]);
            updateText(renderer, &ingameUIElements[12], tempText);
        }
    }    
    else 
    {
        strcpy(tempText, " ");
        updateText(renderer, &ingameUIElements[12], " ");
    }
        
}


void placeTower(int x, int y) 
{
    int newX;
    int newY;

    int tileIndex = returnClosestTile1D(x, y, NULL);

    if(tileIndex == -1)
        return;

    convertClosestTile2D(tileIndex, &newX, &newY);
    
    towerAdd(selectedTower, x, y);
    selectedTower = -2;
}

void removeTower(int x, int y) 
{
    int newX;
    int newY;

    int tileIndex = returnClosestTile1D(x, y, NULL);

    if(tileIndex == -1)
        return;

    convertClosestTile2D(tileIndex, &newX, &newY);

    towerRemove(x, y);
    selectedTower = -2;
}