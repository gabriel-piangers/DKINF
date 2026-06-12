#ifndef MAP_HEADER
#define MAP_HEADER

#include "constants.h"

#define MAP_W 30 //provisorio
#define MAP_H 30 //provisorio
#define TILE_SIZE 24


//Centralizing game drawing constants
extern int mapOffsetX, mapOffsetY;

void LoadMap(int level);
char GetTileAt(int x, int y);
char GetTileAtPos(float posX, float posY);
void DrawMap();
Rectangle CheckCollisionWithTile(Rectangle rect, char tile);

#endif