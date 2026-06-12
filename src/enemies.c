#include "headers/enemies.h"
#include "headers/map.h"

extern Texture2D GHOST_R_TEXTURE;
extern Texture2D GHOST_L_TEXTURE;
extern Texture2D BOULDER_TEXTURE;

Enemy enemies[MAX_ENEMY_AMOUNT];
int enemyAmount = 0;

void InitEnemies(Enemy newEnemies[], int size) {
    Enemy defaultEnemy = {(Vector2) {0.0, 0.0}, GHOST_ENEMY, 0, 0, GHOST_R_TEXTURE, false};
    for (int i=0; i<size; i++) {
        enemies[i] = newEnemies[i];
    }
    for (int i=size; i<MAX_ENEMY_AMOUNT; i++) {
        enemies[i] = defaultEnemy;
    }
    enemyAmount = size;
}

void UpdateEnemies() {
    for (int i = 0; i < MAX_ENEMY_AMOUNT; i++) {
        if (!enemies[i].isAlive) continue;
        
        switch (enemies[i].type) {
        case GHOST_ENEMY: {
            Vector2 nextPosition = (Vector2){enemies[i].position.x + enemies[i].direction * GHOST_SPEED, enemies[i].position.y};

            float front; //para ver o que tem na frente do inimigo
            if (enemies[i].direction == 1) 
                front = nextPosition.x + GHOST_SIZE;
            else
                front = nextPosition.x;

            //verifica se é parede ou se tem chão no próximo passo
            int tileAheadX  = front;
            int tileFeetY   = enemies[i].position.y + GHOST_SIZE - 1;

            char tileAhead = GetTileAtPos(tileAheadX, tileFeetY);
            char tileBelow = GetTileAtPos(tileAheadX, tileFeetY + 1 * TILE_SIZE);

            bool hitWall = (tileAhead == 'Z');
            bool falls = (tileBelow != 'Z');

            if (hitWall || falls) {
                enemies[i].direction *= -1;
                if(enemies[i].direction == 1) enemies[i].texture = GHOST_R_TEXTURE;
                else enemies[i].texture = GHOST_L_TEXTURE;
            } else {
                enemies[i].position = nextPosition;
            }
            break;
        }
        case BOULDER_ENEMY: {
            enemies[i].position.y += GRAVITY * BOULDER_SPEED;
            enemies[i].rotation += 1;

            if(enemies[i].position.y > SCREEN_H + BOULDER_SIZE) {
                enemies[i].isAlive = false;
                enemyAmount--;
            }
        }
        default:
            continue;
        }
    }
    return;
}

void DrawEnemies() {
    for (int i=0; i<MAX_ENEMY_AMOUNT; i++) {
        if(!enemies[i].isAlive) continue;

        Enemy currentEnemy = enemies[i];
        Rectangle srcRec = {0.0, 0.0, currentEnemy.texture.width, currentEnemy.texture.height};

        switch (currentEnemy.type) {
            case GHOST_ENEMY: {
                Rectangle dstRec = {currentEnemy.position.x, currentEnemy.position.y, GHOST_SIZE, GHOST_SIZE};
                Vector2 origin = {0.0, 0.0}; //Usa o canto superior esquerdo como origem
                DrawTexturePro(currentEnemy.texture, srcRec, dstRec, origin, currentEnemy.rotation, WHITE);
                break;
            }                
            case BOULDER_ENEMY: {
                Rectangle dstRec = {currentEnemy.position.x, currentEnemy.position.y, BOULDER_SIZE, BOULDER_SIZE};
                Vector2 origin = {BOULDER_SIZE/2, BOULDER_SIZE/2}; //Usa o centro como origem
                DrawTexturePro(currentEnemy.texture, srcRec, dstRec, origin, currentEnemy.rotation, BROWN);
                break;
            }
            case FIRE_ENEMY: {
                Rectangle dstRec = {currentEnemy.position.x, currentEnemy.position.y, FIRE_SIZE, FIRE_SIZE};
                Vector2 origin = {0.0, 0.0}; //Usa o canto superior esquerdo como origem
                DrawTexturePro(currentEnemy.texture, srcRec, dstRec, origin, currentEnemy.rotation, WHITE);
                break;
            }
        }
    }
}

bool CheckEnemyCollision(Rectangle rectPlayer) { 
    for (int i=0; i<MAX_ENEMY_AMOUNT; i++) {
        if (!enemies[i].isAlive) continue;

        Rectangle rectEnemy = {enemies[i].position.x, enemies[i].position.y, GHOST_SIZE, GHOST_SIZE};
        switch (enemies[i].type) {
            case GHOST_ENEMY:
                rectEnemy.width = GHOST_SIZE; rectEnemy.height = GHOST_SIZE;
                break;
            case BOULDER_ENEMY:
                rectEnemy = (Rectangle) {
                    enemies[i].position.x - BOULDER_SIZE/2,
                    enemies[i].position.y - BOULDER_SIZE/2,
                    BOULDER_SIZE,
                    BOULDER_SIZE,
                };
                break;
            case FIRE_ENEMY:
                rectEnemy.width = FIRE_SIZE; rectEnemy.height = FIRE_SIZE;
                break;
        }
        
        if (CheckCollisionRecs(rectPlayer, rectEnemy)) {
            return true;
        }
        
    }
    
    return false;
}

void SpawnBoulder() {
    Vector2 pos = {mapOffsetX + BOULDER_SIZE + rand() % (MAP_W * TILE_SIZE - BOULDER_SIZE), 0};
    Enemy boulder = {pos, BOULDER_ENEMY, 0, 0, BOULDER_TEXTURE, true};
    
    //Procura por inimigos mortos para fazer a troca no vetor
    for (int i=0; i<MAX_ENEMY_AMOUNT; i++) {
        if(!enemies[i].isAlive) {
            enemies[i] = boulder;
            enemyAmount++;
            return;
        }
    }

    printf("Error spawning boulder: Enemy amount reached the limit!\n");
}