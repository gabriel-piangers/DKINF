#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FPS 60
#define SCREEN_W 900
#define SCREEN_H 675

extern Texture2D FLOOR_TEXTURE;
extern Texture2D STAIR_TEXTURE;
extern Texture2D CHEST_TEXTURE;
extern Texture2D PLAYER_TEXTURE;
extern Texture2D ENEMY_TEXTURE;

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