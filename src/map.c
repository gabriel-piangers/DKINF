#include "headers/map.h"

extern Texture2D FLOOR_TEXTURE;
extern Texture2D STAIR_TEXTURE;
extern Texture2D CHEST_TEXTURE;

char map[MAP_H][MAP_W];

// Centralizing game drawing constants
int mapOffsetX = (SCREEN_W - MAP_W * TILE_SIZE) / 2;
int mapOffsetY = (SCREEN_H - MAP_H * TILE_SIZE) / 2 + 20; //mapOffsetY tem um valor extra para dar espaço para o cabecalho do jogo (timer, level, etc)

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

char GetTileAt(int x, int y) {
    if(x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) {
        return ' ';
    }
    return map[y][x];
}

char GetTileAtPos(float posX, float posY) {
    int x = (posX - mapOffsetX) / TILE_SIZE;
    int y = (posY - mapOffsetY) / TILE_SIZE;
    
    return GetTileAt(x, y);
}


void DrawMap() {
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            switch (map[y][x]) {
            case 'Z': {
                DrawTexture(FLOOR_TEXTURE, x * TILE_SIZE + mapOffsetX, y * TILE_SIZE + mapOffsetY, WHITE);
                break;
            }
            case 'H':
            case 'S': {
                DrawTexture(STAIR_TEXTURE, x * TILE_SIZE + mapOffsetX, y * TILE_SIZE + mapOffsetY, WHITE);
                break;
            }
            case 'F': {
                DrawTexture(CHEST_TEXTURE, x * TILE_SIZE + mapOffsetX, y * TILE_SIZE + mapOffsetY, WHITE);
                break;
            }
            default: {
                break;
            }
            }
        }
    }
}

Rectangle CheckCollisionWithTile(Rectangle rect, char tile) {
    int coordX = (rect.x + rect.width/2 - mapOffsetX)/TILE_SIZE; // coordenada x do centro do retangulo em relação a map
    int coordY = (rect.y + rect.height/2 - mapOffsetY)/TILE_SIZE; // coordenada y do centro do retangulo em relação a map
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = coordX+x, tCoordY = coordY+y;
            if (GetTileAt(tCoordX, tCoordY) == tile) {
                Rectangle tileRect = {mapOffsetX + tCoordX * TILE_SIZE, mapOffsetY + tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                Rectangle overlap = GetCollisionRec(rect, tileRect);
                if(overlap.height > 0 || overlap.width > 0) {
                    return overlap;
                }
            }
        }
    }    
    return (Rectangle) {0, 0, 0, 0};
}