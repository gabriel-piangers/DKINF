#include <raylib.h>
#include "constants.h"
#include "enemies.h"

struct Enemy enemies[ENEMY_AMOUNT]; //provisorio, cada mapa precisa ter exatamente ENEMY_AMOUNT inimigos (usar um vetor dinãmico no futuro)

void InitEnemies(Vector2 positions[]) {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        enemies[i] = (struct Enemy) {positions[i], 1, true};
    }
}

void UpdateEnemies() {
    return; //TODO
}

void DrawEnemies() {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        struct Enemy currentEnemy = enemies[i];
        DrawTexture(ENEMY_TEXTURE, currentEnemy.position.x, currentEnemy.position.y, WHITE);
    }
}

bool CheckEnemyCollision(Rectangle rect) {
    return false; //TODO
}