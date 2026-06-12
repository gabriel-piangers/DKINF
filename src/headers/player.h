#ifndef PLAYER_H
#define PLAYER_H

#include "constants.h"

#define PLAYER_SIZE 16 //Deve ser menor que TILE_SIZE
#define PLAYER_SPEED 1.8
#define PLAYER_JUMP_VELOCITY 9
#define PLAYER_DASH_VELOCITY 6
#define PLAYER_DASH_DURATION 0.2 //seconds !(AIR_RESISTANCE * 2 * FPS * PLAYER_DASH_DURATION deve ser igual a PLAYER_DASH_VELOCITY)!
#define PLAYER_DASH_COOLDOWN 1 //seconds
#define PLAYER_ANIMATION_DURATION 0.6 //seconds
#define PLAYER_ANIMATION_FRAMES 4 //Number of frames 

typedef enum {
    PLAYER_IDLE, 
    PLAYER_RUNNNING,
    PLAYER_CLIMBING,
    PLAYER_DASHING,
    PLAYER_DEAD
} PlayerState;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    PlayerState state;
    Texture2D texture;
    int framePos; //intervalo de [0, PLAYER_RUN_ANIMATION_FRAMES]
    int direction; // 1 -> facing right, -1 facing left
    float dashTimer;
    int lifes;
}  Player;

void InitPlayer(Vector2 pos);
void DrawPlayer();
void UpdatePlayer();
Rectangle GetPlayerRect();
bool PlayerReachedGoal();
bool IsPlayerOnScreen();

#endif
