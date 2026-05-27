#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FPS 60
#define SCREEN_W 900
#define SCREEN_H 675
#define LINE_SPACING 30
#define GRAVITY 0.5 // deve ser maior que AIR_RESISTANCE
#define AIR_RESISTANCE 0.25

// Key inputs
#define KEY_1 KEY_ONE
#define KEY_2 KEY_TWO
#define KEY_3 KEY_THREE
#define KEY_RIGHT KEY_D
#define KEY_LEFT KEY_A
#define KEY_UP KEY_W
#define KEY_DOWN KEY_S
#define KEY_PAUSE KEY_TAB
#define KEY_JUMP KEY_SPACE
#define KEY_DASH KEY_LEFT_SHIFT

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