#ifndef CONSTANTS_H
#define CONSTANTS_H

// Constants Definition
#define FPS 60
#define SCREEN_W 900
#define SCREEN_H 675
#define MAP_W 50 //provisorio
#define MAP_H 27 //provisorio
#define TILE_SIZE 24


// Enums
enum PlayerState {
    PLAYER_IDLE, 
    PLAYER_LETTER,
    PLYER_DEAD
};

enum GameState {
    GAME_PAUSED,
    GAME_LEVEL,
    GAME_MENU,
    GAME_RANKING,
    GAME_OVER,
    GAME_WINNER
};

// Structs
struct Player {
    Vector2 position;
    enum PlayerState state;
    int lifes;
};

#endif