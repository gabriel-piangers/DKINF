#include "ui.h"

void DrawTitle(char title[]) {
    int textSize = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(title, (SCREEN_W - textSize)/2, MARGIN, TITLE_FONT_SIZE, WHITE);
}

void DrawMenu(MenuOption options[]) {
    return; //TODO
}