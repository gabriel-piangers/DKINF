#ifndef UI_H
#define UI_H

#include <raylib.h>
#include "constants.h"

#define LINE_SPACING 30
#define TITLE_FONT_SIZE 48
#define MARGIN 50

typedef struct {
    char text[50];
    GameState state;
} MenuOption;

void DrawTitle(char title[]);
void DrawMenu(MenuOption options[]);

#endif