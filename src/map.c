#include <raylib.h>
#include "map.h"
#include "constants.h"

extern Texture2D FLOOR_TEXTURE;
extern Texture2D STAIR_TEXTURE;
extern Texture2D CHEST_TEXTURE;

char map[MAP_H][MAP_W] = {
        "                                 ",
        "      D        F        D        ",
        "      HZZZZZZZZZZZZZZZZZH        ",
        "      H                 H        ",
        "   D  S                 SD       ",
        "  ZHZZZZZZZZZZZZZZZZZZZZZHZ      ",
        "   H                     H       ",
        "   S     D         D     S       ",
        " ZZZZZZZZHZZZZZZZZZHZZZZZZZZ     ",
        "         H         H             ",
        "  D      S         SE      D     ",
        "ZZHZZZZZZZZ       ZZZZZZZZZH     ",
        "  H                        H     ",
        "  S    D           D       S     ",
        " ZZZZZZHZZZZZZZZZZZHZZZZZZZZ     ",
        "       H           H             ",
        "       S     D     S E           ",
        " ZZZZZZZZZZZZHZZZZZZZZZZZZ       ",
        "             H                   ",
        "      D      S      D            ",
        "     ZHZZZZZZZZZZZZZH            ",
        "      H             H            ",
        "D    ES             S     D      ",
        "HZZZZZZZZZZZZZZZZZZZZZZZZZH      ",
        "H                         H      ",
        "S    P                    S      ",
        "ZZZZZZZZZZZZZZZZZZZZZZZZZZZ      "
    };

void DrawMap() {
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; map[y][x] != '\0'; x++) {
            switch (map[y][x]) {
            case 'Z': {
                DrawTexture(FLOOR_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
                break;
            }
            case 'H':
            case 'S': {
                DrawTexture(STAIR_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
                break;
            }
            case 'F': {
                DrawTexture(CHEST_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
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