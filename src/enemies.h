#ifndef ENEMIES_H
#define ENEMIES_H

#include <raylib.h>

#define GHOST_SIZE 20
#define GHOST_SPEED 1
#define BOULDER_SIZE 28
#define BOULDER_SPEED 4
#define BOULDER_SPAWN_TIME 1.5 // seconds
#define MAX_ENEMY_AMOUNT 50

typedef enum {
    GHOST_ENEMY,
    BOULDER_ENEMY
} EnemyType;

typedef struct {
    Vector2 position;
    EnemyType type;
    int direction; //1 -> moving right, -1 -> moving left, 0 -> No moviment
    float rotation;
    Texture2D texture;
    bool isAlive;
} Enemy;


void InitEnemies(Enemy newEnemies[], int size);
void DrawEnemies();
void UpdateEnemies();
bool CheckEnemyCollision(Rectangle rect); 
void SpawnBoulder();
void UpdateBoulder();
void DrawBoulder();

#endif