#ifndef UI_H
#define UI_H

#include <raylib.h>
#include "constants.h"
#include "io.h"

#define LINE_SPACING 30
#define TITLE_FONT_SIZE 48
#define FONT_SIZE_BIG 36
#define FONT_SIZE_MEDIUM 24
#define FONT_SIZE_SMALL 18
#define MARGIN 50

typedef struct {
    char text[50];
    GameState state;
} MenuOption;

void DrawTitle(char title[]);
void DrawMenu(MenuOption options[], int count);
void DrawInput(char text[], int fontSize, int letterCount, int frameCount);
void DrawRanking(RankScore ranking[]);
void DrawHUD(int level, float timer);

#endif