#ifndef YEARDATA_H
#define YEARDATA_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int year;
    char expensive_game[100];
    float price_expensive_game;
    char cheap_game[100];
    float price_cheap_game;
    int total_games;
    float sum_prices;
    int windows_games;
    int mac_games;
    int linux_games;
    int num_free_games;
    char free_games[50][100];
} YearData;

YearData *createYearData();

char *toString(YearData *data);

float meanPrices(YearData *data);

float getPercentage(int total_games, int plataform_games);

int isEmpty(YearData *data);
#endif