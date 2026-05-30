#ifndef ENEMIES_H
#define ENEMIES_H

#include <raylib.h>

#define ENEMY_SIZE 20
#define PLAYER_SPEED 1
#define ENEMY_AMOUNT 3 //provisorio

typedef struct {
    Vector2 position;
    int direction; //1 -> moving right, -1 -> moving left
    bool isAlive;
} Enemy;

void InitEnemies(Vector2 positions[ENEMY_AMOUNT]);
void DrawEnemies();
void UpdateEnemies();
bool CheckEnemyCollision(Rectangle rect); 


#endif