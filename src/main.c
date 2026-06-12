#include "headers/constants.h"
#include "headers/player.h"
#include "headers/enemies.h"
#include "headers/map.h"
#include "headers/ui.h"
#include "headers/ranking.h"

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
Texture2D GHOST_R_TEXTURE;
Texture2D GHOST_L_TEXTURE;
Texture2D FIRE_TEXTURE;
Texture2D BOULDER_TEXTURE;

//Global variables
GameState currentGameState = GAME_MENU;

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
    GHOST_R_TEXTURE = LoadTexture("assets/ghost_r.png");
    GHOST_L_TEXTURE = LoadTexture("assets/ghost_l.png");
    FIRE_TEXTURE = LoadTexture("assets/fire.png");
    BOULDER_TEXTURE = LoadTexture("assets/boulder.png");
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
    UnloadTexture(GHOST_R_TEXTURE);
    UnloadTexture(GHOST_L_TEXTURE);
    UnloadTexture(FIRE_TEXTURE);
    UnloadTexture(BOULDER_TEXTURE);
}

void InitializeEntities() {
    int enemyIndex = 0;
    Enemy newEnemies[MAX_ENEMY_AMOUNT];
    for (int y = 0; y<MAP_H; y++) {
        for (int x = 0; x<MAP_W; x++) {
            switch (GetTileAt(x, y)) {
            case 'P': {
                int offset = TILE_SIZE - PLAYER_SIZE;
                Vector2 pos = (Vector2) {mapOffsetX + x * TILE_SIZE + offset, mapOffsetY + y * TILE_SIZE + offset};
                InitPlayer(pos);
                break;
            }
            case 'E': {
                int offset = TILE_SIZE - GHOST_SIZE;
                newEnemies[enemyIndex] = (Enemy) {(Vector2){mapOffsetX + x * TILE_SIZE + offset, mapOffsetY + y * TILE_SIZE + offset}, GHOST_ENEMY, 1, 0, GHOST_R_TEXTURE, true};
                enemyIndex++;
                break;
            }
            case 'W': {
                int offset = TILE_SIZE - FIRE_SIZE;
                newEnemies[enemyIndex] = (Enemy) {(Vector2){mapOffsetX + x * TILE_SIZE + offset, mapOffsetY + y * TILE_SIZE + offset}, FIRE_ENEMY, 1, 0, FIRE_TEXTURE, true};
                enemyIndex++;
                break;
            }
            }
        }
    }
    InitEnemies(newEnemies, enemyIndex);
}

void UpdateMenu(MenuOption options[], int count) {
    if (count < 1) return;
    
    if(IsKeyPressed(KEY_1)) {
        currentGameState = options[0].state;
    } else if(IsKeyPressed(KEY_2) && count >= 2) {
        currentGameState = options[1].state;
    } else if(IsKeyPressed(KEY_3) && count >= 3) {
        currentGameState = options[2].state;
    }
}

int main() {
    //Local Variables
    int currentLevel = 1, letterCount = 0, frameCount = 0;
    float timer = 0.0f;
    RankScore playerScore = {"", timer};

    SetTargetFPS(FPS);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "DKINF");
    ToggleBorderlessWindowed();

    //Seed baseada no tempo atual1
    srand(time(NULL)); 

    LoadResources();
    LoadRanking();
    LoadMap(currentLevel);

    InitializeEntities();

    while(!WindowShouldClose()) {
        if(!IsWindowFocused()) {
            MinimizeWindow();
        }

        frameCount++;
        switch (currentGameState) {
            case GAME_MENU: {
                MenuOption options[3] = {
                    { "Novo Jogo", GAME_RESTART},
                    { "Ranking", GAME_RANKING},
                    { "Sair", GAME_QUIT}
                };

                UpdateMenu(options, 3);

                BeginDrawing();
                ClearBackground(BLACK);
                DrawTitle("DKINF");
                
                DrawMenu(options, 3);

                EndDrawing();
                
                break;
            }
            case GAME_RESTART: { // Não adicionar o break!
                timer = 0.0, currentLevel = 1;
                letterCount = 0, frameCount = 0;
                playerScore = (RankScore) {"", 0.0};
                LoadMap(currentLevel);
                InitializeEntities();
                currentGameState = GAME_LEVEL;
            } case GAME_LEVEL: {
                // Update Logic
                timer += GetFrameTime();

                if(frameCount % (int)(BOULDER_SPAWN_TIME * FPS) == 0) SpawnBoulder();

                UpdatePlayer();
                UpdateEnemies();
            
                if (CheckEnemyCollision(GetPlayerRect()) || !IsPlayerOnScreen()) {
                    currentGameState = GAME_OVER;
                    //printf("GAME OVER! FINAL TIME: %.2f SECONDS\n", timer); //
                } else if(PlayerReachedGoal()) {
                    //printf("LEVEL %d FINISHED IN %.2f SECONDS\n", currentLevel, timer);
                    currentLevel++;
                    if (currentLevel > TOTAL_LEVELS) {
                        currentGameState = GAME_FINISHED;
                    } else {
                        LoadMap(currentLevel);
                        InitializeEntities();
                        currentGameState = GAME_LEVEL;
                    }
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
                    { "Novo Jogo", GAME_RESTART},
                    { "voltar ao Menu", GAME_MENU}
                }; 

                UpdateMenu(options, 2);

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME OVER");
               

                DrawMenu(options, 2);

                EndDrawing();

                break;
            }
            case GAME_FINISHED: {
                MenuOption options[] = {
                    {"Novo Jogo", GAME_RESTART},
                    {"Ranking", GAME_RANKING},
                    {"Voltar ao Menu", GAME_MENU}
                };

                int placement = GetScoreIndex((RankScore) {"", timer});
                bool showInput = false;

                if(placement > -1 && playerScore.time == 0.0) {
                    showInput = true;

                    // User input
                    int key = GetKeyPressed();
                    if (key >= 32 && key <= 125 && letterCount < MAX_PLAYER_NAME) {
                        playerScore.name[letterCount] = (char) key;
                        letterCount++;
                    }
                    if(IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                        letterCount--;
                        playerScore.name[letterCount] = '\0';
                    }
                    if(IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                        playerScore.time = timer;
                        SaveScore(playerScore, placement);

                        showInput = false;
                        printf("Player: %s \nTime: %.2f \n", playerScore.name, timer);
                    }
                } else {
                    UpdateMenu(options, 3);
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME FINISHED");
                DrawHUD(currentLevel, timer);

                if(showInput) {
                    DrawInput(playerScore.name, FONT_SIZE_BIG, letterCount, frameCount);
                } else {
                    DrawMenu(options, 3);
                }

                EndDrawing();
                
                break;
            }
            case GAME_PAUSED: {
                MenuOption options[3] = {
                    {"Continuar", GAME_LEVEL},
                    {"Reiniciar", GAME_RESTART},
                    {"Voltar ao menu", GAME_MENU}
                };
                
                if(IsKeyPressed(KEY_PAUSE)) {
                    currentGameState = GAME_LEVEL;
                } else {
                    UpdateMenu(options, 3);
                }

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("GAME PAUSED");
                DrawMenu(options, 3);
                DrawHUD(currentLevel, timer);

                EndDrawing();
                break;
            }
            case GAME_RANKING: {
                MenuOption options[1] = {{"Voltar ao menu", GAME_MENU}};                
                RankScore ranking[SCORE_AMOUNT];

                for(int i=0; i<SCORE_AMOUNT; i++) {
                    ranking[i] = GetScore(i);
                }

                UpdateMenu(options, 1);

                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitle("RANKINGS");                
                DrawRanking(ranking);

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