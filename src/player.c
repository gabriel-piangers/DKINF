#include <raylib.h>
#include <stdio.h>
#include "player.h"
#include "constants.h"
#include "map.h"

extern Texture2D PLAYER_RIGHT_TEXTURE;
extern Texture2D PLAYER_LEFT_TEXTURE;

struct Player player;

void InitPlayer(Vector2 pos) {
    player = (struct Player) {pos, (Vector2) {0.0, 0.0}, PLAYER_IDLE, PLAYER_RIGHT_TEXTURE, 1};
}

void ApplyVelocity() {
    Vector2 newPos = {player.position.x + player.velocity.x, player.position.y + player.velocity.y};
    if(player.velocity.x != 0){
        Rectangle newPlayerRect = {newPos.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
        if (CheckCollisionWithTile(newPlayerRect, 'Z')) {
            newPos.x = player.position.x; //não aplica a velocidade horizontal
            player.velocity.x = 0;
        }
    }
    
    if(player.velocity.y != 0) {
        Rectangle newPlayerRect = {player.position.x, newPos.y, PLAYER_SIZE, PLAYER_SIZE};
        if (CheckCollisionWithTile(newPlayerRect, 'Z')) {
            newPos.y = player.position.y; //não aplica a velocidade vertical
            player.velocity.y = 0;
        }   

        //Checa se o jogador está em cima do fim de uma escada (não pode cair)!
        if(player.state != PLAYER_CLIMBING) {
            int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE;
            int pCoordY = (player.position.y + PLAYER_SIZE)/TILE_SIZE; //usa a parte de baixo do sprite do jogador ao invés do centro
            for (int x=-1; x<=1; x++) { 
                for (int y=-1; y<=1; y++) {
                    int tCoordX = pCoordX+x, tCoordY = pCoordY+y;
                    if (GetTileAt(tCoordX, tCoordY-1) == 'D' && pCoordY < tCoordY) { //se o tile é o último H (fim da escada)
                        Rectangle stairEndRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                        if(CheckCollisionRecs(newPlayerRect, stairEndRect)) {
                            if(player.velocity.y >= 0) {
                                newPos.y = player.position.y; //Não aplica velocidade vertical para baixo (gravidade)
                                player.velocity.y = 0;
                            }
                        } 
                    }
                }
            }
        }
    }
    player.position = newPos;
}

void DrawPlayer() {
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}

void UpdatePlayer() {
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE; // coordenada x do centro do sprite do jogador
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE; // coordenada y do centro do sprite do jogador

    char playerTile = GetTileAt(pCoordX, pCoordY);
    char underTile = GetTileAt(pCoordX, pCoordY+1);

    printf("Player state: %d\n", player.state);

    if(player.state == PLAYER_CLIMBING && playerTile != 'S' && playerTile != 'H' && playerTile != 'D') {
        player.state = PLAYER_IDLE;
    }

    //Gravity aceleration
    if(player.state != PLAYER_CLIMBING) player.velocity.y += GRAVITY;

    //Air resistence
    if(player.velocity.x > 0) {
        player.velocity.x += player.velocity.x > AIR_RESISTANCE ? -AIR_RESISTANCE : -player.velocity.x; 
    } else if (player.velocity.x < 0){
        player.velocity.x += player.velocity.x < -AIR_RESISTANCE ? AIR_RESISTANCE : -player.velocity.x; 
    }
    if(player.velocity.y > 0) {
        player.velocity.y += player.velocity.y > AIR_RESISTANCE ? -AIR_RESISTANCE : -player.velocity.y; 
    } else if(player.velocity.y < 0){
        player.velocity.y += player.velocity.y < -AIR_RESISTANCE ? AIR_RESISTANCE : -player.velocity.y; 
    }

    ApplyVelocity();

    //User input
    if(player.state != PLAYER_DEAD) {
        if(IsKeyDown(KEY_RIGHT)) {
           player.velocity.x = PLAYER_SPEED;
           player.texture = PLAYER_RIGHT_TEXTURE;
        }
        if(IsKeyDown(KEY_LEFT)) {
            player.velocity.x = -(PLAYER_SPEED);    
            player.texture = PLAYER_LEFT_TEXTURE;
        }
        if(IsKeyDown(KEY_UP)) {
            if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                player.state = PLAYER_CLIMBING;
                player.velocity.y = -(PLAYER_SPEED);
            }
        }
        if(IsKeyDown(KEY_DOWN)) {
            if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                player.state = PLAYER_CLIMBING;
                player.velocity.y = PLAYER_SPEED;
            }
        }
        if(IsKeyPressed(KEY_JUMP)) {
            if (underTile == 'Z' || underTile == 'H' || underTile == 'S') { //Jogador está no chão ou na escada
                player.velocity.y = -(PLAYER_JUMP_VELOCITY);
                if (player.state == PLAYER_CLIMBING) player.state = PLAYER_IDLE;
            }
        }
    }
}

Rectangle GetPlayerRect() {
    return (Rectangle) {player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
}

bool PlayerReachedGoal() {
    return CheckCollisionWithTile(GetPlayerRect(), 'F');
}
