#include <raylib.h>
#include <stdio.h>
#include "player.h"
#include "constants.h"
#include "map.h"

extern Texture2D PLAYER_RIGHT_TEXTURE;
extern Texture2D PLAYER_LEFT_TEXTURE;
extern Texture2D PLAYER_JUMP_R_TEXTURE;
extern Texture2D PLAYER_JUMP_L_TEXTURE;
extern Texture2D PLAYER_RUN_R_TEXTURE;
extern Texture2D PLAYER_RUN_L_TEXTURE;

Player player;
float animationTimer = 0; 

void InitPlayer(Vector2 pos) {
    player = (Player) {
        pos, //position
        (Vector2) {0.0, 0.0}, //velocity
        PLAYER_IDLE, //state
        PLAYER_RIGHT_TEXTURE, //texture
        1, //framePos
        PLAYER_DASH_COOLDOWN, //dashTimer
        1 //lifes
    };
}

bool isPlayerOnGround() {
    //tile imediatamente abaixo do canto superior esquerdo do sprite do jogador
    char lUnderTile = GetTileAt(player.position.x/TILE_SIZE, (player.position.y/TILE_SIZE)+1);
    //tile imediatamente abaixo do canto superior direito do sprite do jogador
    char rUnderTile = GetTileAt((player.position.x + PLAYER_SIZE)/TILE_SIZE, (player.position.y/TILE_SIZE)+1);
    return
        player.state == PLAYER_CLIMBING || (
            player.velocity.y <= 1 && player.velocity.y >= -1 && (
                lUnderTile == 'Z' || lUnderTile == 'H' || rUnderTile == 'Z' || rUnderTile == 'H'
            ));
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

    //Aplica velocidade horizontal
    Vector2 newPos = {player.position.x + player.velocity.x, player.position.y + player.velocity.y};
    if(player.velocity.x != 0){
        Rectangle newPlayerRect = {newPos.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
        if (CheckCollisionWithTile(newPlayerRect, 'Z')) {
            newPos.x = player.position.x; //não aplica a velocidade horizontal
            player.velocity.x = 0;
        }
    }
    
    //Aplica velocidade vertical
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

void SetPlayerTexture() {
    bool playerOnGround = isPlayerOnGround();

    if(player.velocity.x > 0) player.direction = 1;
    else if(player.velocity.x < 0) player.direction = -1;


    if(playerOnGround) {
        //Jogador esta no chao ou na escada
        if(player.state == PLAYER_RUNNNING) {
            if(player.direction == 1) player.texture = PLAYER_RUN_R_TEXTURE;
            else player.texture = PLAYER_RUN_L_TEXTURE;
        } else {
            if(player.direction == 1) player.texture = PLAYER_RIGHT_TEXTURE;
            else player.texture = PLAYER_LEFT_TEXTURE;
        }
    } else {
        //Jogador esta no ar
        if(player.direction == 1) player.texture = PLAYER_JUMP_R_TEXTURE;
        else player.texture = PLAYER_JUMP_L_TEXTURE; 
    }
}

void UpdatePlayer() {
    int pCoordX = (player.position.x + PLAYER_SIZE/2)/TILE_SIZE; // coordenada x do centro do sprite do jogador
    int pCoordY = (player.position.y + PLAYER_SIZE/2)/TILE_SIZE; // coordenada y do centro do sprite do jogador

    char playerTile = GetTileAt(pCoordX, pCoordY);
    bool playerOnGround = isPlayerOnGround();

    //Timer updates
    player.dashTimer += GetFrameTime();
    if (player.state == PLAYER_RUNNNING) {
        animationTimer += GetFrameTime();
        if (animationTimer > PLAYER_ANIMATION_DURATION/PLAYER_ANIMATION_FRAMES) {
            animationTimer = 0.0;
            player.framePos++;
            if (player.framePos > PLAYER_ANIMATION_FRAMES) player.framePos = 1;
        }
    }
        
    // player state handlers
    switch (player.state) {
        case PLAYER_IDLE:
            if (player.velocity.x != 0) 
                player.state = PLAYER_RUNNNING;
            break;
        case PLAYER_RUNNNING:
            if(player.velocity.x == 0) 
                player.state = PLAYER_IDLE;
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
                player.velocity.y = -PLAYER_SPEED;
            }
        }
        if(IsKeyDown(KEY_DOWN)) {
            if(playerTile == 'S' || playerTile == 'H' || playerTile == 'D') {
                player.state = PLAYER_CLIMBING;
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
    return CheckCollisionWithTile(GetPlayerRect(), 'F');
}

void DrawPlayer() {
    Rectangle textureRec = {player.framePos * PLAYER_SIZE, 0.0, PLAYER_SIZE, PLAYER_SIZE};
    DrawTextureRec(player.texture, textureRec, player.position, WHITE);
}