#include <raylib.h>
#include <stdio.h>
#include "player.h"
#include "constants.h"
#include "map.h"

extern Texture2D PLAYER_JUMP_R_TEXTURE;
extern Texture2D PLAYER_JUMP_L_TEXTURE;
extern Texture2D PLAYER_RUN_R_TEXTURE;
extern Texture2D PLAYER_RUN_L_TEXTURE;
extern Texture2D PLAYER_TRAIL_R_TEXTURE;
extern Texture2D PLAYER_TRAIL_L_TEXTURE;

Player player;
float animationTimer = 0; 

void InitPlayer(Vector2 pos) {
    player = (Player) {
        pos, //position
        (Vector2) {0.0, 0.0}, //velocity
        PLAYER_IDLE, //state
        PLAYER_RUN_R_TEXTURE, //texture
        0, //framePos
        PLAYER_DASH_COOLDOWN, //dashTimer
        1 //lifes
    };
}

bool isPlayerOnGround() {
    //tile imediatamente abaixo do canto superior esquerdo do sprite do jogador
    char lUnderTile = GetTileAtPos(player.position.x, player.position.y + 1 * TILE_SIZE);
    //tile imediatamente abaixo do canto superior direito do sprite do jogador
    char rUnderTile = GetTileAtPos((player.position.x + PLAYER_SIZE), player.position.y + 1 * TILE_SIZE);
    return
        player.state == PLAYER_CLIMBING || (
            player.velocity.y <= 1 && player.velocity.y >= -1 && (
                lUnderTile == 'Z' || lUnderTile == 'H' || rUnderTile == 'Z' || rUnderTile == 'H'
            ));
}

bool IsPlayerOnScreen() {
    return (player.position.x > 0 && player.position.y > 0 && player.position.x <= SCREEN_W && player.position.y <= SCREEN_H);
}

void ApplyVelocity() {
    //Gravity acceleration
    if(player.state != PLAYER_CLIMBING) player.velocity.y += GRAVITY;

    //Air resistence
    float lateralResistance = player.state == PLAYER_DASHING ? AIR_RESISTANCE*2 : AIR_RESISTANCE; //Dobra a resistencia do ar enquanto faz o dash
    if(player.velocity.x > 0) {
        player.velocity.x += player.velocity.x > lateralResistance ? -lateralResistance : -player.velocity.x; 
    } else if (player.velocity.x < 0){
        player.velocity.x += player.velocity.x < -lateralResistance ? lateralResistance : -player.velocity.x; 
    }
    if(player.velocity.y > 0) {
        player.velocity.y += player.velocity.y > AIR_RESISTANCE ? -AIR_RESISTANCE : -player.velocity.y; 
    } else if(player.velocity.y < 0){
        player.velocity.y += player.velocity.y < -AIR_RESISTANCE ? AIR_RESISTANCE : -player.velocity.y; 
    }

    // Dash estático
    if(player.state == PLAYER_DASHING) player.velocity.y = 0;

    //Aplica velocidade horizontal
    Vector2 newPos = {player.position.x + player.velocity.x, player.position.y + player.velocity.y};
    if(player.velocity.x != 0){
        Rectangle newPlayerRect = {newPos.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
        Rectangle overlap = CheckCollisionWithTile(newPlayerRect, 'Z');
        if (overlap.width > 0) {
            if (player.velocity.x > 0) newPos.x -= overlap.width;
            else newPos.x += overlap.width; 
            player.velocity.x = 0;
        }
    }
    
    //Aplica velocidade vertical
    if(player.velocity.y != 0) {
        Rectangle newPlayerRect = {player.position.x, newPos.y, PLAYER_SIZE, PLAYER_SIZE};
        Rectangle overlap = CheckCollisionWithTile(newPlayerRect, 'Z');
        if (overlap.height > 0) {
            if(player.velocity.y > 0)newPos.y -= overlap.height;
            else newPos.y += overlap.height;
            player.velocity.y = 0;
        }   

        //Checa se o jogador está em cima do fim de uma escada (não pode cair)!
        if(player.state != PLAYER_CLIMBING) {
            int pCoordX = (player.position.x + PLAYER_SIZE/2 - mapOffsetX) / TILE_SIZE;
            int pCoordY = (player.position.y + PLAYER_SIZE - mapOffsetY +1) / TILE_SIZE; //Um pixel a baixo do sprite do jogador
            if (GetTileAt(pCoordX, pCoordY-1) == 'D' && player.velocity.y > 0) { //se o tile é o último H (fim da escada)
                Rectangle tileRect = {mapOffsetX + pCoordX * TILE_SIZE, mapOffsetY + pCoordY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                Rectangle overlap = GetCollisionRec(newPlayerRect, tileRect);
                newPos.y -= overlap.height;
                player.velocity.y = 0;
                }
            } 
    }

    player.position = newPos;
}

void SetPlayerTexture() {
    bool playerOnGround = isPlayerOnGround();

    if(player.velocity.x > 0) player.direction = 1;
    else if(player.velocity.x < 0) player.direction = -1;


    if(playerOnGround) {
        //Jogador esta no chao ou na escada
        if(player.direction == 1) player.texture = PLAYER_RUN_R_TEXTURE;
        else player.texture = PLAYER_RUN_L_TEXTURE;
    } else {
        //Jogador esta no ar
        if(player.direction == 1) player.texture = PLAYER_JUMP_R_TEXTURE;
        else player.texture = PLAYER_JUMP_L_TEXTURE; 
    }
}

void UpdatePlayer() {
    int pCoordX = (player.position.x + PLAYER_SIZE/2); // coordenada x do centro do sprite do jogador
    int pCoordY = (player.position.y + PLAYER_SIZE/2); // coordenada y do centro do sprite do jogador

    char playerTile = GetTileAtPos(pCoordX, pCoordY);
    bool playerOnGround = isPlayerOnGround();

    //Timer updates
    player.dashTimer += GetFrameTime();
    if (player.state == PLAYER_RUNNNING) {
        animationTimer += GetFrameTime();
        if (animationTimer > PLAYER_ANIMATION_DURATION/PLAYER_ANIMATION_FRAMES) {
            animationTimer = 0.0;
            player.framePos++;
            if (player.framePos >= PLAYER_ANIMATION_FRAMES) player.framePos = 0;
        }
    }
        
    // player state handlers
    switch (player.state) {
        case PLAYER_IDLE:
            if (player.velocity.x != 0) 
                player.state = PLAYER_RUNNNING;
            break;
        case PLAYER_RUNNNING:
            if(player.velocity.x == 0) {
                player.state = PLAYER_IDLE;
                player.framePos = 0;
            }
            break;
        case PLAYER_CLIMBING:
            if(playerTile != 'S' && playerTile != 'H' && playerTile != 'D') 
                player.state = PLAYER_IDLE;
            break;
        case PLAYER_DASHING:
            if(player.dashTimer >= PLAYER_DASH_DURATION) {
                player.state = PLAYER_IDLE;
                player.dashTimer = 0;
            }
            break;
        default:
            break;
    }

    //User input
    if(player.state != PLAYER_DEAD) {
        if(IsKeyDown(KEY_RIGHT) && player.state != PLAYER_DASHING) {
           player.velocity.x = PLAYER_SPEED;
        }
        if(IsKeyDown(KEY_LEFT) && player.state != PLAYER_DASHING) {
            player.velocity.x = -PLAYER_SPEED;    
        }
        if(IsKeyDown(KEY_UP)) {
            if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                player.state = PLAYER_CLIMBING;
                player.framePos = 0;
                player.velocity.y = -PLAYER_SPEED;
            }
        }
        if(IsKeyDown(KEY_DOWN)) {
            if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                player.state = PLAYER_CLIMBING;
                player.framePos = 0;
                player.velocity.y = PLAYER_SPEED;
            }
        }
        if(IsKeyPressed(KEY_JUMP)) {
            if (playerOnGround) { //Jogador está no chão ou na escada
                player.velocity.y = -PLAYER_JUMP_VELOCITY;
                if (player.state == PLAYER_CLIMBING) player.state = PLAYER_IDLE;
            }
        }
        if(IsKeyPressed(KEY_DASH) && player.dashTimer >= PLAYER_DASH_COOLDOWN) {
            player.velocity.x += player.velocity.x >= 0 ? PLAYER_DASH_VELOCITY : -PLAYER_DASH_VELOCITY;
            player.state = PLAYER_DASHING;
            player.dashTimer = 0;
        }
    }

    ApplyVelocity();

    SetPlayerTexture();
}

Rectangle GetPlayerRect() {
    return (Rectangle) {player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
}

bool PlayerReachedGoal() {
    Rectangle overlap = CheckCollisionWithTile(GetPlayerRect(), 'F');
    return (overlap.width > 0 || overlap.height > 0);
}

void DrawPlayer() {
    Rectangle textureRec = {player.framePos * PLAYER_SIZE, 0.0, PLAYER_SIZE, PLAYER_SIZE};
    Color playerColor = WHITE;
    if(player.state != PLAYER_DASHING && player.dashTimer < PLAYER_DASH_COOLDOWN) playerColor = (Color) {255, 255, 255, 180};

    DrawTextureRec(player.texture, textureRec, player.position, playerColor);

    //Dash trail
    Color trailColor = (Color) {255, 255, 255, 180};
    if(player.state == PLAYER_DASHING) {
        if(player.direction == 1) {
            DrawTexture(PLAYER_TRAIL_R_TEXTURE, player.position.x - PLAYER_SIZE/2, player.position.y, trailColor );
        } else {
            DrawTexture(PLAYER_TRAIL_L_TEXTURE, player.position.x + PLAYER_SIZE, player.position.y, trailColor);
        }
    }
}