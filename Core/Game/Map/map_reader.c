#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_reader.h"

int *path;
int pathSize;
int *textureID;

void mapInit(char *fileName, int *gridSizeX, int *gridSizeY) 
{
    char filePath[99] = "Maps/";
    strcat(filePath, fileName);
    strcat(filePath, ".mapData");
     
    FILE *mapFile = fopen(filePath, "r");

    char line[99];
    int section = 0;
    int pathIndex = -1;
    int textureIndex = 0;
    
    while(fgets(line, sizeof(line), mapFile) !=  NULL && section < 3) 
    {           
        if(line[0] == '#') 
        {
            section++;
            continue;
        }
            
        switch(section) 
        {
            case 0: 
            {
                int i = 0;
                int foundX = 0;
                while(line[i] != '\n' && line[i] != '\0') 
                {
                    if(line[i] == 'x') 
                    {
                        foundX++;
                        break;
                    }
                    i++;
                }             
                
                if(foundX == 0)                    
                {
                    *gridSizeX = atoi(line);
                    *gridSizeY = *gridSizeX;
                }
                else 
                {
                    *gridSizeX = atoi(strtok(line, "x"));
                    *gridSizeY = atoi(strtok(NULL, "x")); 
                }
                    
                textureID = malloc(sizeof(int) * (*gridSizeX) * (*gridSizeY));
                break;                                                    
            }
            case 1:
            {
                if(pathIndex == -1) 
                {
                    pathSize = atoi(line);
                    path = malloc(sizeof(int) * 2 * pathSize);                    
                    pathIndex++;
                    break;
                }

                path[pathIndex++] = atoi(strtok(line, ","));
                path[pathIndex++] = atoi(strtok(NULL, ","));
                break;
            }            
            case 2: 
            {   
                char* lineToken = strtok(line, ",");     
                while (lineToken != NULL && lineToken[0] != '\0')
                { 
                    textureID[textureIndex++] = atoi(lineToken);
                    lineToken = strtok(NULL, ",");
                }
                break;
            }
        }            
    }
    fclose(mapFile);
}
void freeMapData() 
{
    free(path);
    free(textureID);
}