#ifndef USER_INTERFACE_C
#define USER_INTERFACE_C
#include "SDL2/SDL_ttf.h"

extern TTF_Font *font;

typedef struct 
{
    
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    SDL_Rect area;    
    int id;
    
}  UIElement;

extern UIElement mainMenuElements[4];
extern UIElement ingameUIElements[24];
extern UIElement uiEndScreenElements[24];
extern int uiEndScreenElementsAmount;
extern int mainMenuElementAmount;
extern int ingameUIElementAmount;
extern int currentScene;
extern int gameRunning;

void mainMenuInit(SDL_Renderer *renderer, int windowX, int windowY);
void createUIElement(SDL_Renderer *renderer, UIElement *uiElements, int *index, int xPos, int yPos, int width, int height, SDL_Texture *texture, char *text, SDL_Color textColor);
int buttonIndex(int mouseX, int mouseY, UIElement *uiElements, int size);
void mainMenubuttonLogic(int index, SDL_Renderer *renderer, int windowX, int windowY);
void renderUI(SDL_Renderer *renderer, UIElement *uiElements, int size);
void updateText(SDL_Renderer *renderer, UIElement *uiElement, char *newText);
void uiEndScreenInit(SDL_Renderer *renderer, int windowX, int windowY);
void freeUI(UIElement *uiElements, int size);

#endif