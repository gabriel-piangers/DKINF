#include "ui.h"
#include <string.h>

void DrawTitle(char title[]) {
    int textSize = MeasureText(title, TITLE_FONT_SIZE);
    DrawText(title, (SCREEN_W - textSize)/2, MARGIN, TITLE_FONT_SIZE, WHITE);
}

void DrawMenu(MenuOption options[], int count) {
    int longerStringPos = 0;

    for(int i = 0; i < count; i++) {
        if(strlen(options[i].text) > strlen(options[longerStringPos].text)) {
            longerStringPos = i;
        } 
    }

    for (int j= 0; j < count; j++){
        int textSize = MeasureText(options[longerStringPos].text, FONT_SIZE_MEDIUM);
        DrawText(TextFormat("%d. %s", j+1, options[j].text), (SCREEN_W - textSize) / 2, SCREEN_H / 2 + j * LINE_SPACING, FONT_SIZE_MEDIUM, WHITE);
    }
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

void DrawRanking(RankScore ranking[]) {
    int fontSize = FONT_SIZE_MEDIUM, letterSize = MeasureText("A", fontSize);
    int posX = (SCREEN_W - letterSize*38)/2;
    int posY = (SCREEN_H - fontSize*SCORE_AMOUNT)/2;
    
    for (int i=0; i<SCORE_AMOUNT; i++) {
        int offsetY = (fontSize+5) * i;
        DrawText(TextFormat("%d.", i+1), posX, posY + offsetY, fontSize, WHITE);
        DrawText(ranking[i].name, posX + letterSize*4, posY + offsetY, fontSize, WHITE);
        DrawText(TextFormat("%.2f", ranking[i].time), posX + letterSize*30, posY + offsetY, fontSize, WHITE);
    }

    DrawText("1. Voltar ao menu", posX, posY + (fontSize + 5) * (SCORE_AMOUNT+1), fontSize, WHITE);
}

void DrawHUD(int level, float timer) {
    DrawText(TextFormat("Level: %d", level), 20, 20, FONT_SIZE_SMALL, RED);
    DrawText(TextFormat("Time: %.2f", timer), 100, 20, FONT_SIZE_SMALL, RED);
}
