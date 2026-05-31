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
#include "ui.h"

// Define textures
Texture2D FLOOR_TEXTURE;
Texture2D STAIR_TEXTURE;
Texture2D CHEST_TEXTURE;
Texture2D PLAYER_RIGHT_TEXTURE;
Texture2D PLAYER_LEFT_TEXTURE;
Texture2D PLAYER_JUMP_R_TEXTURE;
Texture2D PLAYER_JUMP_L_TEXTURE;
Texture2D PLAYER_RUN_R_TEXTURE; // 1x4 sprite sheet
Texture2D PLAYER_RUN_L_TEXTURE; // 1x4 sprite sheet
Texture2D ENEMY_TEXTURE;

// Game functions
void LoadResources() {
    FLOOR_TEXTURE = LoadTexture("assets/floor.png");
    STAIR_TEXTURE = LoadTexture("assets/stair.png");
    CHEST_TEXTURE = LoadTexture("assets/chest.png");
    PLAYER_RIGHT_TEXTURE = LoadTexture("assets/player_right.png");
    PLAYER_LEFT_TEXTURE = LoadTexture("assets/player_left.png");
    PLAYER_JUMP_R_TEXTURE = LoadTexture("assets/player_jump_r.png");
    PLAYER_JUMP_L_TEXTURE = LoadTexture("assets/player_jump_l.png");
    PLAYER_RUN_R_TEXTURE = LoadTexture("assets/player_run_r.png");
    PLAYER_RUN_L_TEXTURE = LoadTexture("assets/player_run_l.png");
    ENEMY_TEXTURE = LoadTexture("assets/fire.png");
}

void UnloadResources() {
    UnloadTexture(FLOOR_TEXTURE);
    UnloadTexture(STAIR_TEXTURE);
    UnloadTexture(CHEST_TEXTURE);
    UnloadTexture(PLAYER_RIGHT_TEXTURE);
    UnloadTexture(PLAYER_LEFT_TEXTURE);
    UnloadTexture(PLAYER_JUMP_R_TEXTURE);
    UnloadTexture(PLAYER_JUMP_L_TEXTURE);
    UnloadTexture(PLAYER_RUN_R_TEXTURE);
    UnloadTexture(PLAYER_RUN_L_TEXTURE);
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
    int currentLevel = 0, letterCount = 0, frameCount = 0;
    float timer = 0.0f;
    char playerName[MAX_PLAYER_NAME] = "";
    GameState currentGameState = GAME_MENU;

    InitializeEntities();

    while(!WindowShouldClose()) {
        frameCount++;

        switch (currentGameState) {
            case GAME_MENU: {
                if (IsKeyPressed(KEY_1)) {
                    InitializeEntities();
                    currentGameState = GAME_LEVEL;
                } else if (IsKeyPressed(KEY_2)) {
                    currentGameState = GAME_RANKING;
                } else if (IsKeyPressed(KEY_3)) {
                    UnloadResources();
                    CloseWindow();
                    return 0; // Foi obrigado a usar, se não da problema 
                }

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("DKINF");

                DrawText("1. Novo Jogo", SCREEN_W / 2, SCREEN_H / 2, 20, WHITE);
                DrawText("2. Ranking", SCREEN_W / 2, SCREEN_H / 2 + LINE_SPACING, 20, WHITE);
                DrawText("3. Sair", SCREEN_W / 2, SCREEN_H / 2 + 2 * LINE_SPACING, 20, WHITE);

                EndDrawing();
                
                break;
            }
            case GAME_LEVEL: {
                // Update Logic
                timer += GetFrameTime();
                UpdatePlayer();
                UpdateEnemies();
            
                if (CheckEnemyCollision(GetPlayerRect())) {
                    currentGameState = GAME_OVER;
                    //printf("GAME OVER! FINAL TIME: %.2f SECONDS\n", timer); //
                }

                if(PlayerReachedGoal()) {
                    printf("LEVEL %d FINISHED IN %.2f SECONDS\n", currentLevel, timer);
                    currentGameState = GAME_FINISHED;
                }
                
                BeginDrawing();
                ClearBackground(BLACK);
                DrawMap();

                DrawEnemies();
                DrawPlayer();

                DrawHUD(currentLevel, timer);
                EndDrawing();

                break;
            }
            case GAME_OVER: {
                if (IsKeyPressed(KEY_1)) {
                    InitializeEntities();
                    timer = 0.0f;
                    currentLevel = 0;
                    currentGameState = GAME_LEVEL;
                } else if (IsKeyPressed(KEY_2)) {
                    currentGameState = GAME_MENU;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("GAME OVER");

                DrawText("1. Reiniciar", SCREEN_W / 2, SCREEN_H / 2 + 2 *LINE_SPACING, 20, WHITE);
                DrawText("2. Menu", SCREEN_W / 2, SCREEN_H / 2 + 3 * LINE_SPACING, 20, WHITE);
                EndDrawing();

                break;
            }
            case GAME_FINISHED: {
                //User input
                int key = GetKeyPressed();
                if (key >= 32 && key <= 125 && letterCount < MAX_PLAYER_NAME) {
                    playerName[letterCount] = (char) key;
                    letterCount++;
                }
                if(IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                    letterCount--;
                    playerName[letterCount] = '\0';
                }
                if(IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                    RankScore newScore = {(char) {' '}, timer}; // não da pra passar a string diretamente :(
                    strcpy(newScore.PlayerName, playerName);

                    printf("Player: %s \nTime: %.2f \n", playerName, timer); //Salvar isso em ranking.bin

                    currentGameState = GAME_RANKING;
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME FINISHED");

                DrawInput(playerName, FONT_SIZE_BIG, letterCount, frameCount);

                EndDrawing();
                
                break;
            }
            case GAME_PAUSED: {

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("GAME PAUSED");
                EndDrawing();
            }
            case GAME_RANKING: {

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("RANKINGS");
                EndDrawing();
            }
        }
    }

    UnloadResources();
    CloseWindow();

    return 0;
}