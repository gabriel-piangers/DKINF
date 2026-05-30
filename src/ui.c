#include "ui.h"

void DrawTitle(char title[]) {
    int textSize = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(title, (SCREEN_W - textSize)/2, MARGIN, TITLE_FONT_SIZE, WHITE);
}

void DrawMenu(MenuOption options[]) {
    return; //TODO
}

void DrawInput(char text[], int fontSize, int letterCount, int frameCount) {
    int padding = fontSize/2;
    int letterSize = MeasureText("Z", fontSize)+4; //por algum motivo MeasureText nao calcula certo
    int inputWidth =  letterSize * MAX_PLAYER_NAME + padding*2; 
    int inputHeight = fontSize + padding*2;

    DrawRectangle((SCREEN_W - inputWidth)/2, (SCREEN_H - inputHeight)/2, inputWidth, inputHeight, LIGHTGRAY);
    DrawText(text, (SCREEN_W - inputWidth)/2 + padding, (SCREEN_H - inputHeight)/2 + padding, fontSize, RED);
    if((frameCount/20) % 2 == 0 && letterCount < MAX_PLAYER_NAME) 
        DrawRectangle(
            (SCREEN_W - inputWidth)/2 + padding + MeasureText(text, fontSize) + 5,
            (SCREEN_H - inputHeight)/2 + padding + fontSize - 5,
            20, 5, RED
        );
}