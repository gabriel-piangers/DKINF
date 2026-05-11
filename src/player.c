#include <raylib.h>
#include "player.h"
#include "constants.h"
#include "map.h"

extern Texture2D PLAYER_RIGHT_TEXTURE;
extern Texture2D PLAYER_LEFT_TEXTURE;

struct Player player;

void InitPlayer(Vector2 pos) {
    player = (struct Player) {pos, PLAYER_IDLE, PLAYER_RIGHT_TEXTURE, 1};
}

bool PlayerCanMoveTo(Vector2 pos) { 
    Rectangle newPlayerRect = {pos.x, pos.y, PLAYER_SIZE, PLAYER_SIZE};
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE;
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE;
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = pCoordX+x, tCoordY = pCoordY+y;
            if (GetTileAt(tCoordX, tCoordY) == 'Z') { //se colidir com alguma plataforma
                Rectangle platformRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(CheckCollisionRecs(newPlayerRect, platformRect)) return false; 
            }
        }
    }    
    return true;
}

bool ApplyGravity() {
    Vector2 pos = {player.position.x, player.position.y+GRAVITY};
    if (player.state == PLAYER_CLIMBING || !PlayerCanMoveTo(pos)) return false;

    Rectangle newPlayerRect = {pos.x, pos.y, PLAYER_SIZE, PLAYER_SIZE};
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE;
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE;
    for (int x=-1; x<=1; x++) { 
        for (int y=-1; y<=1; y++) {
            int tCoordX = pCoordX+x, tCoordY = pCoordY+y;
            if (GetTileAt(tCoordX, tCoordY-1) == 'D' && pCoordY < tCoordY) { //se colidir com o fim da escada por cima
                Rectangle stairEndRect = {tCoordX * TILE_SIZE, tCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                if(CheckCollisionRecs(newPlayerRect, stairEndRect)) return false; 
            }
        }
    }
    player.position = pos;
    return true;
}

void DrawPlayer() {
    DrawTexture(player.texture, player.position.x, player.position.y, WHITE);
}

void UpdatePlayer() {
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE;
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE;

    if(player.state == PLAYER_CLIMBING && GetTileAt(pCoordX, pCoordY) != 'S' && GetTileAt(pCoordX, pCoordY) != 'H' && GetTileAt(pCoordX, pCoordY) != 'D') {
        player.state = PLAYER_IDLE;
    }

    ApplyGravity();

    if(player.state != PLAYER_DEAD) {
        if(IsKeyDown(KEY_RIGHT)) {
            Vector2 newPos =  {player.position.x + PLAYER_SPEED, player.position.y};
            if (PlayerCanMoveTo(newPos)) {
                player.position = newPos;
                player.texture = PLAYER_RIGHT_TEXTURE;
            }
        }
        if(IsKeyDown(KEY_LEFT)) {
            Vector2 newPos =  {player.position.x - PLAYER_SPEED, player.position.y};
            if (PlayerCanMoveTo(newPos)) {
                player.position = newPos;
                player.texture = PLAYER_LEFT_TEXTURE;
            }
        }
        if(IsKeyDown(KEY_UP)) {
            Vector2 newPos =  {player.position.x, player.position.y - PLAYER_SPEED};
            if(PlayerCanMoveTo(newPos)) {
                if(GetTileAt(pCoordX, pCoordY) == 'S' || GetTileAt(pCoordX, pCoordY) == 'H' || GetTileAt(pCoordX, pCoordY) == 'D') {
                    player.state = PLAYER_CLIMBING;
                    player.position = newPos;
                }
            }
        }
        if(IsKeyDown(KEY_DOWN)) {
            Vector2 newPos = {player.position.x, player.position.y + PLAYER_SPEED};
            if(PlayerCanMoveTo(newPos)) {
                if(GetTileAt(pCoordX, pCoordY) == 'S' || GetTileAt(pCoordX, pCoordY) == 'H' || GetTileAt(pCoordX, pCoordY) == 'D') {
                    player.state = PLAYER_CLIMBING;
                    player.position = newPos;
                }
            }
        }
    }
}

Rectangle GetPlayerRect() {
    return (Rectangle) {player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
}

bool PlayerReachedGoal() {
    return false; //TODO
}
