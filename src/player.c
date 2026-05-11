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

bool CheckPlayerCollisionWithTile(Vector2 pos, char tile) {
    Rectangle newPlayerRect = {pos.x, pos.y, PLAYER_SIZE, PLAYER_SIZE};
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE; // coordenada x do centro do sprite do jogador
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE; // coordenada y do centro do sprite do jogador
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = pCoordX+x, tCoordY = pCoordY+y;
            if (GetTileAt(tCoordX, tCoordY) == tile) {
                Rectangle platformRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(CheckCollisionRecs(newPlayerRect, platformRect)) return true; 
            }
        }
    }    
    return false;
}

bool ApplyVelocity() {
    Vector2 newPos = {player.position.x + player.velocity.x, player.position.y + player.velocity.y};
    if (player.state == PLAYER_CLIMBING || CheckPlayerCollisionWithTile(newPos, 'Z')) return false;

    Rectangle newPlayerRect = {newPos.x, newPos.y, PLAYER_SIZE, PLAYER_SIZE};
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE;
    int pCoordY = (player.position.y + PLAYER_SIZE)/TILE_SIZE; //usa a parte de baixo do sprite do jogador ao invés do centro
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = pCoordX+x, tCoordY = pCoordY+y;
            if (GetTileAt(tCoordX, tCoordY-1) == 'D' && pCoordY < tCoordY) { //se colidir com o fim da escada por cima
                Rectangle stairEndRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(CheckCollisionRecs(newPlayerRect, stairEndRect)) return false; 
            }
        }
    }
    player.position = newPos;
    return true;
}

void DrawPlayer() {
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}

void UpdatePlayer() {
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE; // coordenada x do centro do sprite do jogador
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE; // coordenada y do centro do sprite do jogador

    char playerTile = GetTileAt(pCoordX, pCoordY);

    printf("Player state: %d\n", player.state);

    if(player.state == PLAYER_CLIMBING && playerTile != 'S' && playerTile != 'H' && playerTile != 'D') {
        player.state = PLAYER_IDLE;
    }

    //Gravity aceleration
    player.velocity.y += GRAVITY;

    if(!ApplyVelocity()) player.velocity = (Vector2) {0.0, 0.0}; //Reset 

    if(player.state != PLAYER_DEAD) {
        if(IsKeyDown(KEY_RIGHT)) {
            Vector2 newPos =  {player.position.x + PLAYER_SPEED, player.position.y};
            if (!CheckPlayerCollisionWithTile(newPos, 'Z')) {
                player.position = newPos;
                player.texture = PLAYER_RIGHT_TEXTURE;
            }
        }
        if(IsKeyDown(KEY_LEFT)) {
            Vector2 newPos =  {player.position.x - PLAYER_SPEED, player.position.y};
            if (!CheckPlayerCollisionWithTile(newPos, 'Z')) {
                player.position = newPos;
                player.texture = PLAYER_LEFT_TEXTURE;
            }
        }
        if(IsKeyDown(KEY_UP)) {
            Vector2 newPos =  {player.position.x, player.position.y - PLAYER_SPEED};
            if(!CheckPlayerCollisionWithTile(newPos, 'Z')) {
                if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                    player.state = PLAYER_CLIMBING;
                    player.position = newPos;
                }
            }
        }
        if(IsKeyDown(KEY_DOWN)) {
            Vector2 newPos = {player.position.x, player.position.y + PLAYER_SPEED};
            if(!CheckPlayerCollisionWithTile(newPos, 'Z')) {
                if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                    player.state = PLAYER_CLIMBING;
                    player.position = newPos;
                }
            }
        }
        if(IsKeyPressed(KEY_JUMP)) {
            char underTile = GetTileAt(pCoordX, pCoordY+1);
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
    return CheckPlayerCollisionWithTile(player.position, 'F');
}
