#include <stdio.h>
#include <raylib.h>
#include "map.h"
#include "constants.h"

extern Texture2D FLOOR_TEXTURE;
extern Texture2D STAIR_TEXTURE;
extern Texture2D CHEST_TEXTURE;

char map[MAP_H][MAP_W];

// Centralizing game drawing constants
int offsetX = (SCREEN_W - MAP_W * TILE_SIZE) / 2;
int offsetY = (SCREEN_H - MAP_H * TILE_SIZE) / 2 + 50; //offsetY tem um valor extra para dar espaço para o cabecalho do jogo (timer, level, etc)

void LoadMap(int level) {
    int lin = 0, col = 0, c;

    FILE *mapFile;
    mapFile = fopen(TextFormat("maps/mapa%d.txt", level), "r");

    for (int i = 0; i < MAP_H; i++) {
        for (int j = 0; j < MAP_W; j++) {
            map[i][j] = ' '; // limpa os lixos de memoria
        }
    }

    if(!(mapFile == NULL)) {
        while((c =  fgetc(mapFile)) != EOF){
            switch (c){
            case '\n':
                lin++;
                col = 0;
                break;
            case ' ':
                map[lin][col] = ' ';
                col++;
                break;
            default:
                map[lin][col] = (char) c;
                col++;
                break;
            }
        }
        
    } else{
        printf("Could not open map file\n");
    }

    fclose(mapFile);
}

void DrawMap() {
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            switch (map[y][x]) {
            case 'Z': {
                DrawTexture(FLOOR_TEXTURE, x * TILE_SIZE + offsetX, y * TILE_SIZE + offsetY, WHITE);
                break;
            }
            case 'H':
            case 'S': {
                DrawTexture(STAIR_TEXTURE, x * TILE_SIZE + offsetX, y * TILE_SIZE + offsetY, WHITE);
                break;
            }
            case 'F': {
                DrawTexture(CHEST_TEXTURE, x * TILE_SIZE + offsetX, y * TILE_SIZE + offsetY, WHITE);
                break;
            }
            default: {
                break;
            }
            }
        }
    }
}

char GetTileAt(int x, int y) {
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return ' ';
    return map[y][x];
}

bool CheckCollisionWithTile(Rectangle rect, char tile) {
    int coordX = (rect.x + rect.width/2)/TILE_SIZE; // coordenada x do centro do retangulo
    int coordY = (rect.y + rect.height/2)/TILE_SIZE; // coordenada y do centro do retangulo
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = coordX+x, tCoordY = coordY+y;
            if (GetTileAt(tCoordX, tCoordY) == tile) {
                Rectangle platformRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(CheckCollisionRecs(rect, platformRect)) return true; 
            }
        }
    }    
    return false;
}