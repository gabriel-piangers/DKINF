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
Texture2D PLAYER_JUMP_R_TEXTURE;
Texture2D PLAYER_JUMP_L_TEXTURE;
Texture2D PLAYER_RUN_R_TEXTURE; // 1x4 sprite sheet
Texture2D PLAYER_RUN_L_TEXTURE; // 1x4 sprite sheet
Texture2D PLAYER_TRAIL_R_TEXTURE;
Texture2D PLAYER_TRAIL_L_TEXTURE;
Texture2D ENEMY_TEXTURE;

// Game functions
void LoadResources() {
    FLOOR_TEXTURE = LoadTexture("assets/floor.png");
    STAIR_TEXTURE = LoadTexture("assets/stair.png");
    CHEST_TEXTURE = LoadTexture("assets/chest.png");
    PLAYER_JUMP_R_TEXTURE = LoadTexture("assets/player_jump_r.png");
    PLAYER_JUMP_L_TEXTURE = LoadTexture("assets/player_jump_l.png");
    PLAYER_RUN_R_TEXTURE = LoadTexture("assets/player_run_r.png");
    PLAYER_RUN_L_TEXTURE = LoadTexture("assets/player_run_l.png");
    PLAYER_TRAIL_R_TEXTURE = LoadTexture("assets/player_trail_r.png");
    PLAYER_TRAIL_L_TEXTURE = LoadTexture("assets/player_trail_l.png");
    ENEMY_TEXTURE = LoadTexture("assets/fire.png");
}

void UnloadResources() {
    UnloadTexture(FLOOR_TEXTURE);
    UnloadTexture(STAIR_TEXTURE);
    UnloadTexture(CHEST_TEXTURE);
    UnloadTexture(PLAYER_JUMP_R_TEXTURE);
    UnloadTexture(PLAYER_JUMP_L_TEXTURE);
    UnloadTexture(PLAYER_RUN_R_TEXTURE);
    UnloadTexture(PLAYER_RUN_L_TEXTURE);
    UnloadTexture(PLAYER_TRAIL_R_TEXTURE);
    UnloadTexture(PLAYER_TRAIL_L_TEXTURE);
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
                MenuOption options[3] = {
                    { "Novo Jogo", GAME_RESTART},
                    { "Ranking", GAME_RANKING},
                    { "Sair", GAME_QUIT}
                };

                if (IsKeyPressed(KEY_1)) {
                    currentGameState = options[0].state;
                } else if (IsKeyPressed(KEY_2)) {
                    currentGameState = options[1].state;
                } else if (IsKeyPressed(KEY_3)) {
                    currentGameState = options[2].state;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("DKINF");
                
                DrawMenu(options, 3);

                EndDrawing();
                
                break;
            }
            case GAME_RESTART: { // Não adicionar o break!
                timer = 0.0;
                currentLevel = 0;
                InitializeEntities();
                currentGameState = GAME_LEVEL;
            } case GAME_LEVEL: {
                // Update Logic
                timer += GetFrameTime();
                UpdatePlayer();
                UpdateEnemies();
            
                if (CheckEnemyCollision(GetPlayerRect())) {
                    currentGameState = GAME_OVER;
                    //printf("GAME OVER! FINAL TIME: %.2f SECONDS\n", timer); //
                } else if(PlayerReachedGoal()) {
                    printf("LEVEL %d FINISHED IN %.2f SECONDS\n", currentLevel, timer);
                    currentGameState = GAME_FINISHED;
                } else if(IsKeyPressed(KEY_PAUSE)) {
                    currentGameState = GAME_PAUSED;
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
                MenuOption options[2] = {
                    { "Reiniciar", GAME_RESTART},
                    { "Menu", GAME_MENU}
                }; 

                if (IsKeyPressed(KEY_1)) {
                    currentGameState = options[0].state;
                } else if (IsKeyPressed(KEY_2)) {
                    currentGameState = options[1].state;
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME OVER");
               

                DrawMenu(options, 2);

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
                MenuOption options[2] = {{"Continuar", GAME_LEVEL}, {"Voltar ao menu", GAME_MENU}};
                
                if(IsKeyPressed(KEY_PAUSE)) {
                    currentGameState = GAME_LEVEL;
                } else if(IsKeyPressed(KEY_1)) {
                    currentGameState = options[0].state;
                } else if(IsKeyPressed(KEY_2)){
                    currentGameState = options[1].state;
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME PAUSED");
                DrawMenu(options, 2);
                DrawHUD(currentLevel, timer);

                EndDrawing();
                break;
            }
            case GAME_RANKING: {
                MenuOption options[1] = {{"Voltar ao menu", GAME_MENU}};

                if (IsKeyPressed(KEY_1)) {
                    currentGameState = options[0].state;
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("RANKINGS");
                DrawMenu(options, 1);

                EndDrawing();
                break;
            }
            case GAME_QUIT: {
                UnloadResources();
                CloseWindow();
                return 0;
            }
        }
    }

    UnloadResources();
    CloseWindow();

    return 0;
}