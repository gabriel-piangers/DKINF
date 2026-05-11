#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "player.h"
#include "enemies.h"
#include "map.h"

// Define textures
Texture2D FLOOR_TEXTURE;
Texture2D STAIR_TEXTURE;
Texture2D CHEST_TEXTURE;
Texture2D PLAYER_RIGHT_TEXTURE;
Texture2D PLAYER_LEFT_TEXTURE;
Texture2D ENEMY_TEXTURE;

// Game functions
void LoadResources() {
    FLOOR_TEXTURE = LoadTexture("assets/floor.png");
    STAIR_TEXTURE = LoadTexture("assets/stair.png");
    CHEST_TEXTURE = LoadTexture("assets/chest.png");
    PLAYER_RIGHT_TEXTURE = LoadTexture("assets/player_right.png");
    PLAYER_LEFT_TEXTURE = LoadTexture("assets/player_left.png");
    ENEMY_TEXTURE = LoadTexture("assets/fire.png");
}

void UnloadResources() {
    UnloadTexture(FLOOR_TEXTURE);
    UnloadTexture(STAIR_TEXTURE);
    UnloadTexture(CHEST_TEXTURE);
    UnloadTexture(PLAYER_RIGHT_TEXTURE);
    UnloadTexture(PLAYER_LEFT_TEXTURE);
    UnloadTexture(ENEMY_TEXTURE);
}

void InitializeEntities() {
    int enemyIndex = 0;
    Vector2 enemyPositions[ENEMY_AMOUNT];
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; x<MAP_W; x++) {
            switch (GetTileAt(x, y)) {
            case 'P': {
                int offset = TILE_SIZE - PLAYER_SIZE;
                Vector2 pos = (Vector2) {x * TILE_SIZE + offset, y * TILE_SIZE + offset};
                InitPlayer(pos);
                break;
            }
            case 'E': {
                int offset = TILE_SIZE - ENEMY_SIZE;
                enemyPositions[enemyIndex] = (Vector2) {x * TILE_SIZE + offset, y * TILE_SIZE + offset};
                enemyIndex++;
                break;
            }
            }
        }
    }
    InitEnemies(enemyPositions);
}

void DrawHUD(int level, float timer) {
    DrawText(TextFormat("Level: %d", level), 20, 20, 16, RED);
    DrawText(TextFormat("Time: %.2f", timer), 100, 20, 16, RED);
}

int main() {
    SetTargetFPS(FPS);
    InitWindow(SCREEN_W, SCREEN_H, "DKINF");

    LoadResources();

    //Variables
    int currentLevel = 0;
    float timer = 0.0f;

    InitializeEntities();

    while(!WindowShouldClose()) {
        // Update Logic
        timer += GetFrameTime();
        UpdatePlayer();
        UpdateEnemies();

        if (CheckEnemyCollision(GetPlayerRect())) {
            printf("GAME OVER"); //TODO
        }

        if(PlayerReachedGoal()) {
            printf("LEVEL %d FINISHED IN %.2f SECONDS", currentLevel, timer); // TODO
        }

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