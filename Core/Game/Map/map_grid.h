#ifndef MAP_GRID_H
#define MAP_GRID_H

typedef struct 
{
    int tileType;
    int placedTowerID;
    int origin;
} Tile;

extern Tile *tileMap;
extern int gridSizeX;
extern int gridSizeY;
extern int tileArea;
extern int middleX;
extern int middleY;

void mapGridInit(int windowSizeX, int windowSizeY, SDL_Renderer *renderer);
void drawGrid(SDL_Renderer *renderer, int mouseX, int mouseY);
int returnClosestTile1D(int x, int y, double *distance);
void returnTileRealLocation(int tileX, int tileY, int *x, int *y);
void returnTileRealLocationFloat(int tileX, int tileY, double *x, double *y); 
int convertClosestTile1D(int x, int y);
void convertClosestTile2D(int index, int *x, int *y);
void returnAllTileLocationData(int positionX, int positionY, int *index, int *xPos, int *yPos);
void freeGrid();

#endif