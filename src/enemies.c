#include <raylib.h>
#include "constants.h"
#include "enemies.h"
#include "map.h"

extern Texture2D ENEMY_TEXTURE;

Enemy enemies[ENEMY_AMOUNT]; //provisorio, cada mapa precisa ter exatamente ENEMY_AMOUNT inimigos (usar um vetor dinãmico no futuro)
extern char GetTileAt(int x, int y);

void InitEnemies(Vector2 positions[]) {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        enemies[i] = (Enemy) {positions[i], 1, true};
    }
}

void UpdateEnemies() {
    for (int i = 0; i < ENEMY_AMOUNT; i++) {
        if (!enemies[i].isAlive) continue;
        
        Vector2 nextPosition = (Vector2){enemies[i].position.x + enemies[i].direction * PLAYER_SPEED, enemies[i].position.y};

        float front; //para ver o que tem na frente do inimigo
        if (enemies[i].direction == 1) 
            front = nextPosition.x + ENEMY_SIZE;
        else
            front = nextPosition.x;

        //verifica se é parede ou se tem chão no próximo passo
        int tileAheadX  = (int)(front / TILE_SIZE);
        int tileFeetY   = (int)((enemies[i].position.y + ENEMY_SIZE - 1) / TILE_SIZE);

        char tileAhead = GetTileAt(tileAheadX, tileFeetY);
        char tileBelow = GetTileAt(tileAheadX, tileFeetY + 1);

        bool hitWall = (tileAhead == 'Z');
        bool falls = (tileBelow != 'Z');

        if (hitWall || falls) {
            enemies[i].direction *= -1;
        } else {
            enemies[i].position = nextPosition;
        }
    }
    return; //TODO
}

void DrawEnemies() {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        Enemy currentEnemy = enemies[i];
        DrawTexture(ENEMY_TEXTURE, currentEnemy.position.x, currentEnemy.position.y, WHITE);
    }
}

bool CheckEnemyCollision(Rectangle rectPlayer) {
    for (int i=0; i<ENEMY_AMOUNT; i++) {
        
        if (enemies[i].isAlive){
            Rectangle rectEnemy = {enemies[i].position.x, enemies[i].position.y, ENEMY_SIZE, ENEMY_SIZE};
            
            if (CheckCollisionRecs(rectPlayer, rectEnemy)) {
                return true;
            }
        }
    }
    
    return false; //TODO
}