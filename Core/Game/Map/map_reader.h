#ifndef MAP_READER_H
#define MAP_READER_H

extern int *path; 
extern int pathSize;
extern int *textureID;

void mapInit(char *fileName, int *gridSizeX, int *gridSizeY);
void freeMapData();

#endif