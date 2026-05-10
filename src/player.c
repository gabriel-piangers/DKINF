#include <raylib.h>
#include "player.h"
#include "constants.h"

struct Player player;

void InitPlayer(Vector2 pos) {
    player = (struct Player) {pos, PLAYER_IDLE, 1};
}

void DrawPlayer() {
    DrawTexture(PLAYER_TEXTURE, player.position.x, player.position.y, WHITE);
}

void UpdatePlayer() {
    player.position.x += 1; //TODO
}

Rectangle GetPlayerRect() {
    return (Rectangle) {player.position.x, player.position.y, PLAYER_SIZE, PLAYER_SIZE};
}

bool PlayerReachedGoal() {
    return false; //TODO
}
