#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "user_interface.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_test_font.h"
#include "../Game/Enemy/enemy_logic.h"
#include "../Game/Map/map_grid.h"
#include "../Game/Towers/tower_logic.h"
#include "../Game/gameloop_logic.h"


TTF_Font *font;
UIElement mainMenuElements[4];
UIElement ingameUIElements[24];
UIElement uiEndScreenElements[24];
SDL_Texture *ingameUITextures[10];
int mainMenuElementAmount = 0;
int ingameUIElementAmount = 0;
int uiEndScreenElementsAmount = 0;
int gameRunning = 1;
int currentScene = 0;

void mainMenuInit(SDL_Renderer *renderer, int windowX, int windowY) 
{
    font = TTF_OpenFont("Assets/Fonts/Tiny5/Tiny5-Regular.ttf", 32);

    char line[99];
    FILE *file = fopen("Maps/highscore.txt", "r");
    int highscore = atoi(fgets(line, sizeof(line), file));

    fclose(file);

    sprintf(line, "HighScore: %d", highscore);
    createUIElement(renderer, mainMenuElements, &mainMenuElementAmount, windowX / 4, windowY * .1, windowX / 2, windowX * 0.1, NULL,  "Burger Defender", (SDL_Color){255,255,255,255});
    createUIElement(renderer, mainMenuElements, &mainMenuElementAmount, windowX / 2 - mainMenuElements[0].area.w / 4, windowY * .35 + mainMenuElements[0].area.y, mainMenuElements[0].area.w / 2, windowX * 0.1, NULL,  "Play", (SDL_Color){0,255,0,255});
    createUIElement(renderer, mainMenuElements, &mainMenuElementAmount, windowX / 2 - mainMenuElements[0].area.w / 4, windowY * .6 + mainMenuElements[1].area.y / 4, mainMenuElements[0].area.w / 2, windowX * 0.1, NULL, "Exit", (SDL_Color){255,0,0,255}); 
    createUIElement(renderer, mainMenuElements, &mainMenuElementAmount, mainMenuElements[0].area.x + mainMenuElements[0].area.w / 4 , mainMenuElements[0].area.y + mainMenuElements[0].area.h + mainMenuElements[0].area.h * 0.1, mainMenuElements[0].area.w / 2, windowX * 0.1, NULL, line, (SDL_Color){255,255,255,255});  
}

void ingameUIInit(SDL_Renderer *renderer, int windowX, int windowY) 
{
    ingameUITextures[0] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/coin.png");
    ingameUITextures[1] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/black.png");
    ingameUITextures[2] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/heart.png");
    ingameUITextures[3] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/score.png");

    ingameUITextures[4] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/basic.png");
    ingameUITextures[5] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/strong.png");
    ingameUITextures[6] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/cube.png");
    ingameUITextures[7] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/sniper.png");
    ingameUITextures[8] = IMG_LoadTexture(renderer, "Assets/Sprites/UI/destroy.png");
    
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount, 0, 0, windowX, tileArea, ingameUITextures[1], NULL, (SDL_Color){255,255,255,255});
    
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount, 0, 0, tileArea, tileArea, ingameUITextures[0], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer, ingameUIElements, &ingameUIElementAmount, tileArea + tileArea * 0.1, 0, tileArea * 1.5, tileArea, NULL, "6767", (SDL_Color){255,255,255,255});    
    
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[2].area.x + ingameUIElements[2].area.w, 0, tileArea, tileArea, ingameUITextures[2], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer, ingameUIElements, &ingameUIElementAmount, ingameUIElements[3].area.x + ingameUIElements[3].area.w + tileArea *0.1, 0, tileArea, tileArea, NULL, "67", (SDL_Color){255,255,255,255});

    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[4].area.x + ingameUIElements[4].area.w, 0, tileArea, tileArea, ingameUITextures[3], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer, ingameUIElements, &ingameUIElementAmount, ingameUIElements[5].area.x + ingameUIElements[5].area.w + tileArea *0.1, 0, tileArea * 2, tileArea, NULL, "67676767", (SDL_Color){255,255,255,255});

    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,windowX - tileArea, 0, tileArea, tileArea, ingameUITextures[7], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[7].area.x - ingameUIElements[7].area.w - tileArea * 0.05, 0, tileArea, tileArea, ingameUITextures[6], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[8].area.x - ingameUIElements[8].area.w - tileArea * 0.05, 0, tileArea, tileArea, ingameUITextures[5], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[9].area.x - ingameUIElements[9].area.w - tileArea * 0.05, 0, tileArea, tileArea, ingameUITextures[4], NULL, (SDL_Color){255,255,255,255});
    createUIElement(renderer,ingameUIElements,&ingameUIElementAmount,ingameUIElements[10].area.x - ingameUIElements[10].area.w - tileArea * 0.05, 0, tileArea, tileArea, ingameUITextures[8], NULL, (SDL_Color){255,255,255,255});

    createUIElement(renderer, ingameUIElements, &ingameUIElementAmount, windowX / 2 - tileArea, 0, tileArea * 2, tileArea, NULL, "Cost : 999C", (SDL_Color){255,255,255,255});
}
void uiEndScreenInit(SDL_Renderer *renderer, int windowX, int windowY) 
{
    char tempText[99];

    if(health > 0) 
        createUIElement(renderer,uiEndScreenElements,&uiEndScreenElementsAmount, windowX / 4, windowY * .1, windowX / 2, windowX * 0.1, NULL,  "You Win!", (SDL_Color){0,255,0,255});
    else 
        createUIElement(renderer,uiEndScreenElements,&uiEndScreenElementsAmount, windowX / 4, windowY * .1, windowX / 2, windowX * 0.1, NULL,  "You Lose", (SDL_Color){255,0,0,255});
    
    sprintf(tempText,"Score: %ld",score);
    

    createUIElement(renderer,uiEndScreenElements,&uiEndScreenElementsAmount, windowX / 2 - windowX / 4, uiEndScreenElements[0].area.y + uiEndScreenElements[0].area.h, windowX / 2,  uiEndScreenElements[0].area.y + uiEndScreenElements[0].area.h, NULL, tempText, (SDL_Color){255,255,255,255});

    char line[99];
    FILE *file = fopen("Maps/highscore.txt", "r");
    int highscore = atoi(fgets(line, sizeof(line), file));
    fclose(file);
    if(highscore < score) 
    {
        file = fopen("Maps/highscore.txt", "w");
        fprintf(file ,"%ld",score);
        fclose(file);
    }  
}
void renderUI(SDL_Renderer *renderer, UIElement *uiElements, int size) 
{
    for(int i = 0; i < size; i++) 
        SDL_RenderCopy(renderer, uiElements[i].textTexture, NULL, &uiElements[i].area);
}

int buttonIndex(int mouseX, int mouseY, UIElement *uiElements, int size) 
{
    int closestIndex = -1;
    double closestDistance = INT32_MAX;
    for(int i = 0; i < size; i++) 
    {
        double distance = sqrt(pow(mouseX - (uiElements[i].area.x + uiElements[i].area.w / 2), 2) + pow(mouseY - (uiElements[i].area.y + uiElements[i].area.h / 2), 2));

        if(closestDistance > distance) 
        {
            closestIndex = i;
            closestDistance = distance;
        }
    }
        
    if(closestIndex == -1)
        return closestIndex;

    SDL_Point mousePos = {mouseX, mouseY}; //points would have been useful to know about when writing path[] lol
    if(SDL_PointInRect(&mousePos, &uiElements[closestIndex].area))
        return closestIndex;
    
    return -1;
}

void mainMenubuttonLogic(int index, SDL_Renderer *renderer, int windowX, int windowY)
{
    switch(index) 
    {
        case -1:
            return;
            break;
        case 1: 
        {
            currentScene = 1;                
            enemyInit(renderer);
            mapGridInit(windowX, windowY, renderer);    
            towerInit(renderer);
            ingameUIInit(renderer, windowX, windowY);

        }     
        break;
        case 2:
            gameRunning = 0;
            break;           
    }
}

void createUIElement(SDL_Renderer *renderer, UIElement *uiElements, int *index, int xPos, int yPos, int width, int height, SDL_Texture *texture, char *text, SDL_Color textColor)
{
    uiElements[*index].area = (SDL_Rect){xPos, yPos, width, height};

    if(texture != NULL) 
    {
        
        uiElements[*index].textTexture = texture;
        uiElements[*index].id = *index;
        (*index)++;

        return;
    }

    uiElements[*index].textSurface = TTF_RenderText_Blended(font, text, textColor);
    uiElements[*index].textTexture = SDL_CreateTextureFromSurface(renderer, uiElements[*index].textSurface);
    uiElements[*index].id = *index;
    (*index)++;
}

void updateText(SDL_Renderer *renderer, UIElement *uiElement, char *newText)
{
    if (uiElement->textSurface)
        SDL_FreeSurface(uiElement->textSurface);
    if (uiElement->textTexture)
        SDL_DestroyTexture(uiElement->textTexture);

    uiElement->textSurface = TTF_RenderText_Blended(font, newText, (SDL_Color){255,255,255,255});
    uiElement->textTexture = SDL_CreateTextureFromSurface(renderer, uiElement->textSurface);

    uiElement->area.w = uiElement->textSurface->w;
    uiElement->area.h = tileArea;
}
void freeUI(UIElement *uiElements, int size)
{
    for(int i = 0; i < size; i++) 
    {
        SDL_FreeSurface(uiElements[i].textSurface);
        SDL_DestroyTexture(uiElements[i].textTexture);
    }  
}