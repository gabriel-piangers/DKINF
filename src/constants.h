#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define FPS 60
#define SCREEN_W 1600
#define SCREEN_H 900
#define GRAVITY 0.5 // deve ser maior que AIR_RESISTANCE
#define AIR_RESISTANCE 0.25
#define MAX_PLAYER_NAME 20

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

// Enums & Structs
typedef enum {
    GAME_PAUSED,
    GAME_LEVEL,
    GAME_MENU,
    GAME_RANKING,
    GAME_OVER,
    GAME_FINISHED,
    GAME_RESTART, // Equivalente a GAME_LEVEL, porém com lógica de restart
    GAME_QUIT // Fecha o jogo de maneira segura
} GameState;

#endif