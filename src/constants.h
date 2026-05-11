#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FPS 60
#define SCREEN_W 900
#define SCREEN_H 675
#define GRAVITY 4

// Key inputs
#define KEY_RIGHT KEY_D
#define KEY_LEFT KEY_A
#define KEY_UP KEY_W
#define KEY_DOWN KEY_S
#define KEY_PAUSE KEY_TAB

// Enums
enum GameState {
    GAME_PAUSED,
    GAME_LEVEL,
    GAME_MENU,
    GAME_RANKING,
    GAME_OVER,
    GAME_WINNER
};

#endif