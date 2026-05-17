#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_SIZE 16
#define PLAYER_SPEED 2
#define PLAYER_JUMP_VELOCITY 4.5

enum PlayerState {
    PLAYER_IDLE, 
    PLAYER_CLIMBING,
    PLAYER_DEAD
};

struct Player {
    Vector2 position;
    Vector2 velocity;
    enum PlayerState state;
    Texture2D texture;
    int lifes;
};

void InitPlayer(Vector2 pos);
void DrawPlayer();
void UpdatePlayer();
Rectangle GetPlayerRect();
bool PlayerReachedGoal();

#endif
