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
Texture2D PLAYER_TEXTURE;
Texture2D ENEMY_TEXTURE;

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
struct Enemy enemies[ENEMY_AMOUNT]; //provisorio, cada mapa precisa ter exatamente ENEMY_AMOUNT inimigos (usar um vetor dinãmico no futuro)

// Game functions
void LoadResources() {
    FLOOR_TEXTURE = LoadTexture("assets/floor.png");
    STAIR_TEXTURE = LoadTexture("assets/stair.png");
    CHEST_TEXTURE = LoadTexture("assets/chest.png");
    PLAYER_TEXTURE = LoadTexture("assets/player0.png");
    ENEMY_TEXTURE = LoadTexture("assets/fire.png");
}

void UnloadResources() {
    UnloadTexture(FLOOR_TEXTURE);
    UnloadTexture(STAIR_TEXTURE);
    UnloadTexture(CHEST_TEXTURE);
    UnloadTexture(PLAYER_TEXTURE);
    UnloadTexture(ENEMY_TEXTURE);
}

void DrawMap() {
    int enemyIndex = 0;
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
            case 'P': {
                int offset = TILE_SIZE - PLAYER_SIZE;
                player.position = (Vector2) {x * TILE_SIZE + offset, y * TILE_SIZE + offset};
                break;
            }
            case 'E': {
                int offset = TILE_SIZE - ENEMY_SIZE;
                enemies[enemyIndex] = (struct Enemy) {(Vector2) {x * TILE_SIZE + offset, y * TILE_SIZE + offset}, 1};
                enemyIndex++;
                break;
            }
            default: {
                break;
            }
            }
        }
    }
}

void DrawHUD(int level, float timer) {
    DrawText(TextFormat("Level: %d", level), 20, 20, 16, RED);
    DrawText(TextFormat("Time: %.2f", timer), 100, 20, 16, RED);
}

void DrawPlayer() {
    DrawTexture(PLAYER_TEXTURE, player.position.x, player.position.y, WHITE);
}

void DrawEnemies() {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        struct Enemy currentEnemy = enemies[i];
        DrawTexture(ENEMY_TEXTURE, currentEnemy.position.x, currentEnemy.position.y, WHITE);
    }
}

int main() {
    SetTargetFPS(FPS);
    InitWindow(SCREEN_W, SCREEN_H, "DKINF");
    
    LoadResources();

    //Variables
    int currentLevel = 0;
    float timer = 0.0f;

    while(!WindowShouldClose()) {
        // Update Logic
        timer += GetFrameTime();

        // Drawing Logic
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawMap();

        DrawPlayer();
        DrawEnemies();

        DrawHUD(currentLevel, timer);

        EndDrawing();
    }

    UnloadResources();
    CloseWindow();
}