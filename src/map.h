#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "constants.h"

#define MAP_W 35 //provisorio
#define MAP_H 27 //provisorio
#define TILE_SIZE 24


//Centralizing game drawing constants
extern int mapOffsetX, mapOffsetY;

void LoadMap(int level);
char GetTileAt(int x, int y);
char GetTileAtPos(float posX, float posY);
void DrawMap();
bool CheckCollisionWithTile(Rectangle rect, char tile);

#endif