#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"

//Game Constants
Texture2D FLOOR_TEXTURE;
Texture2D STAIR_TEXTURE;
Texture2D CHEST_TEXTURE;

//Global Variables
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
        "  S     D           D      S     ",
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

struct Player player = {(Vector2) {0, 0}, PLAYER_IDLE, 1};
float timer = 0.0f;

// Game functions
void loadResources() {
    FLOOR_TEXTURE = LoadTexture("assets/floor.png");
    STAIR_TEXTURE = LoadTexture("assets/stair.png");
    CHEST_TEXTURE = LoadTexture("assets/chest.png");
}

void unloadResources() {
    UnloadTexture(FLOOR_TEXTURE);
    UnloadTexture(STAIR_TEXTURE);
    UnloadTexture(CHEST_TEXTURE);
}

void DrawMap() {
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; map[y][x] != '\0'; x++) {
            switch (map[y][x]) {
            case 'Z':
                DrawTexture(FLOOR_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
                break;
            case 'H':
            case 'S':
                DrawTexture(STAIR_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
                break;
            case 'F':
                DrawTexture(CHEST_TEXTURE, x * TILE_SIZE, y * TILE_SIZE, WHITE);
                break;
            default:
                break;
            }
        }
    }
}

void DrawHUD(int level) {
    DrawText(TextFormat("Level: %d", level), 20, 20, 16, RED);
    DrawText(TextFormat("Time: %.2f", timer), 100, 20, 16, RED);
}

int main() {
    SetTargetFPS(FPS);
    InitWindow(SCREEN_W, SCREEN_H, "DKINF");

    loadResources();

    //Variables
    int currentLevel = 0;

    while(!WindowShouldClose()) {
        // Update Logic
        timer += GetFrameTime();

        // Drawing Logic
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawMap();
        DrawHUD(currentLevel);

        EndDrawing();
    }

    unloadResources();

    CloseWindow();
}