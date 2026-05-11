#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_SIZE 16
#define PLAYER_SPEED 3

enum PlayerState {
    PLAYER_IDLE, 
    PLAYER_CLIMBING,
    PLAYER_DEAD
};

struct Player {
    Vector2 position;
    enum PlayerState state;
    Texture2D texture;
    int lifes;
};

void InitPlayer(Vector2 pos);
void DrawPlayer();
void UpdatePlayer();
Rectangle GetPlayerRect();
bool CheckPlayerCollisionWithTile(Vector2 pos, char tile);
bool PlayerReachedGoal();

#endif
