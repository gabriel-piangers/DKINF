#ifndef MAP_H
#define MAP_H

#include <raylib.h>
#include "constants.h"

#define MAP_W 35 //provisorio
#define MAP_H 27 //provisorio
#define TILE_SIZE 24


//Centralizing game drawing constants
extern int offsetX, offsetY;

void LoadMap(int level);
void DrawMap();
char GetTileAt(int x, int y);
bool CheckCollisionWithTile(Rectangle rect, char tile);

#endif