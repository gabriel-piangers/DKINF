#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_SIZE 16

enum PlayerState {
    PLAYER_IDLE, 
    PLAYER_CLIMBING,
    PLYER_DEAD
};

struct Player {
    Vector2 position;
    enum PlayerState state;
    int lifes;
};

void InitPlayer(Vector2 pos);
void DrawPlayer();
void UpdatePlayer();
Rectangle GetPlayerRect();
bool PlyerReachedGoal();

#endif
