#include "yearData.h"

// Funciones hebra madre
void generateThreads(int num_threads);

char *yearsDataToString(YearData **years_data);

void writeOutputFile(char *file_name, char *years_data);

// Funciones hebra hija
void daughterThread();

int getYear(char *line);

void updateYearData(char *line, YearData *year_data); // SC | line (?
