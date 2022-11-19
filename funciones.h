#include "yearData.h"

// Funciones hebra madre:

YearData *createYearsDataArray(int initial_year);

void writeOutputFile(char *file_name, YearData *years_data, int initial_year); // *years_data puede cambiar a **years_data

void printYearsData(YearData *years_data, int initial_year); // *years_data puede cambiar a **years_data

void printThreadsProcessedLines(int *processed_lines, int num_threads);

// Funciones hebra hija:

int getYear(char *game_data);

void updateYearData(char *game_data, YearData *year_data, float min_price, int initial_year);
