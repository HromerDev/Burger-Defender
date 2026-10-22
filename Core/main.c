#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "dynamic_array.h"
#include "Game/Towers/tower_logic.h"
#include "Game/Map/map_grid.h"
#include <SDL2/SDL_ttf.h>
#include "Game/Enemy/enemy_logic.h"
#include "UI/user_interface.h"
#include "Game/gameloop_logic.h"
#include "string.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#include <emscripten/emscripten.h>
#endif

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Event event;

int windowX;
int windowY;

double deltaTime = 0;
Uint64 last = 0;

int mouseX = 0;
int mouseY = 0;

int currentButton = -1;

void processDeltaTime(double *deltaTime, Uint64 *last) 
{
    Uint64 now = SDL_GetPerformanceCounter();
    *deltaTime = (double)((now - *last) / (double)SDL_GetPerformanceFrequency());
    *last = now;
}

int initialized = 0;

void goFullscreen()
{
#ifdef __EMSCRIPTEN__
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#else
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
}

void gameLoop(void)
{
    if(!initialized)
    {
        mainMenuInit(renderer, windowX, windowY);
        waveInit();
        initialized = 1;
    }

    processDeltaTime(&deltaTime, &last);    
    SDL_GetMouseState(&mouseX, &mouseY);

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:

                gameRunning = 0;

                #ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
                #endif
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    if(currentScene == 1)
                    {
                        if(selectedTower == -2)
                            selectTower(mouseX, mouseY);
                        else if(selectedTower >= 0)
                            placeTower(mouseX, mouseY);
                        else if(selectedTower == -1)
                            removeTower(mouseX, mouseY);
                    }
                    else
                    {
                        currentButton = buttonIndex(
                            mouseX, mouseY,
                            mainMenuElements,
                            mainMenuElementAmount
                        );
                    }
                }

                if(event.button.button == SDL_BUTTON_RIGHT)
                    selectedTower = -2;

                break;
            case SDL_KEYDOWN:
                #ifdef __EMSCRIPTEN__
                    if(event.key.keysym.sym == SDLK_F11)
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                #else
                    if(event.key.keysym.sym == SDLK_F11)
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                #endif
                
                break;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch(currentScene)
    {
        case 0:
            renderUI(renderer, mainMenuElements, mainMenuElementAmount);
            mainMenubuttonLogic(currentButton, renderer, windowX, windowY);
            break;

        case 1:
            spawnWaves(deltaTime);
            textUpdating(renderer, mouseX, mouseY);

            moveEnemies(deltaTime);
            rotateTowers();
            projectileMove(deltaTime);
            processTowerShootCooldown(deltaTime);

            drawGrid(renderer, mouseX, mouseY);
            renderProjectiles(renderer);

            renderUI(renderer, ingameUIElements, ingameUIElementAmount);

            enemyKill();
            updateWaveProgress(renderer, windowX, windowY);
            drawEnemies(renderer);

            if(health <= 0)
            {
                currentScene = 2;
                uiEndScreenInit(renderer, windowX, windowY);
            }
            break;

        case 2:
            renderUI(renderer, uiEndScreenElements, uiEndScreenElementsAmount);
            break;
    }

    currentButton = buttonIndex(mouseX, mouseY, ingameUIElements, ingameUIElementAmount);

    SDL_RenderPresent(renderer);

    #ifndef __EMSCRIPTEN__
    if(!gameRunning)
        return;
    #endif
}


int main(int argc, char** argv)
{
    srand((int)time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    windowX = 1280;
    windowY = 720;

    window = SDL_CreateWindow("TD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, SDL_WINDOW_SHOWN);

    #ifdef __EMSCRIPTEN__
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(renderer, 1280, 720);
    #else
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    #endif

    #ifdef __EMSCRIPTEN__
        last = SDL_GetPerformanceCounter();
        emscripten_set_main_loop(gameLoop, 0, 1);
    #else
        last = SDL_GetPerformanceCounter();
        while(gameRunning) gameLoop();
    #endif

    #ifndef __EMSCRIPTEN__
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    #endif

    return 0;
}