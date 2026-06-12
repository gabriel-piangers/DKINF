#include "headers/ranking.h"

RankScore defaultScore = {"No entry", INFINITY};
RankScore rankingScores[SCORE_AMOUNT] = {0};

void LoadRanking() {
    //Inicializa o ranking vazio
    for(int i=0; i<SCORE_AMOUNT; i++) {
        rankingScores[i] = defaultScore;
    }

    FILE *f;
    f = fopen(RANKING_FILE_PATH, "rb");
    if(f != NULL) {
        if(fread(rankingScores, sizeof(RankScore), SCORE_AMOUNT, f) != sizeof(RankScore) * SCORE_AMOUNT) {
            printf("Error reading scores from %s!\n", RANKING_FILE_PATH);
        }
        fclose(f);
    } else {
        printf("Could not open ranking file at path: %s\n", RANKING_FILE_PATH);
    }
}

void SaveRankings() {
    FILE *f;
    f = fopen(RANKING_FILE_PATH, "wb");
    if(f != NULL) {
        if(fwrite(rankingScores, sizeof(RankScore), SCORE_AMOUNT, f) != sizeof(RankScore)*SCORE_AMOUNT) {
            printf("Error writing scores in %s\n!", RANKING_FILE_PATH);
        }
        fclose(f);
    } else {
        printf("Error saving rankings at path: %s\n!", RANKING_FILE_PATH);
    }
}

void SaveScore(RankScore newScore, int index) {
    if(index<0 || index>=SCORE_AMOUNT) {
        printf("Index out of range for SaveScore!\n");
        return;
    }

    RankScore prevScore, currentScore = newScore;
    for(int i=index; i<SCORE_AMOUNT; i++) {
        prevScore = rankingScores[i];
        rankingScores[i] = currentScore;
        currentScore = prevScore;
    }

    SaveRankings();
}

int GetScoreIndex(RankScore score) {
    for (int i=0; i<SCORE_AMOUNT; i++) {
        if(score.time < rankingScores[i].time) {
            return i;
        }
    }
    return -1; // Score não entra pro ranking
}

RankScore GetScore(int index) {
    if(index<0 || index>=SCORE_AMOUNT) {
        printf("Index out of range for GetScores!\n");
        return defaultScore;
    }
    return rankingScores[index];
}