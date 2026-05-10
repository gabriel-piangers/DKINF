#ifndef MAP_H
#define MAP_H

#include <raylib.h>

#define MAP_W 35 //provisorio
#define MAP_H 27 //provisorio
#define TILE_SIZE 24

void DrawMap();
char GetTileAt(int x, int y);

#endif