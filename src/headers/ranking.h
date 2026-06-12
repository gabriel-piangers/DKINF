#ifndef RANKING_H
#define RANKING_H

#include "constants.h"

#define SCORE_AMOUNT 10
#define RANKING_FILE_PATH "bin/placar.bin"

typedef struct {
    char name[MAX_PLAYER_NAME];
    float time;
} RankScore;

void LoadRanking();
void SaveScore(RankScore newScore, int index);
int GetScoreIndex(RankScore score);
RankScore GetScore(int index);

#endif