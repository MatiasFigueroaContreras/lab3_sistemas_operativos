#include "funciones.h"

// Funciones hebra madre:

/*
    Entradas:
        - int initial_year: Anio inicial para comenzar a buscar informacion.
    Salidas:
        - YearData *years_data: Arreglo con las estructuras YearData creadas
            e inicializadas.
    Descripcion:
        - Funcion encargada de crear un arreglo de estructuras
            YearData segun el anio inicial dado hasta el anio actual.
*/
YearData *createYearsDataArray(int initial_year)
{

    int num_years = 2022 - initial_year + 1;
    int index;
    YearData *years_data = malloc(sizeof(YearData) * num_years);
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        years_data[index] = *createYearData();
        years_data[index].year = y;
        years_data[index].price_expensive_game = -1;
        years_data[index].price_cheap_game = 1000000;
    }

    return years_data;
}

/*
    Entradas:
        -char *file_name: Nombre del archivo de salida.
        -YearData *years_data: Arreglo con la informacion referente
            a cada anio.
        -int initial_year: anio inicial de los datos.
    Salidas:
        -void
    Descripcion:
        -Funcion destinada a escribir en un archivo de salida la
            informacion referente a cada anio.
*/
void writeOutputFile(char *file_name, YearData *years_data, int initial_year)
{
    int index;
    int num_years = 2022 - initial_year + 1;
    FILE *file = fopen(file_name, "w");
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        if (!isEmpty(&years_data[index]))
        {
            // El toString se encarga de calcular los porcentajes de plataforma y el promedio de precios
            fputs(toString(&years_data[index]), file); 
        }
    }
}

/*
    Entradas:
        - YearData *years_data: Arreglo con la informacion referente
            a cada anio.
    Salidas:
        - void
    Descripcion:
        - Imprime por consola la informacion referente a cada anio.
*/
void printYearsData(YearData *years_data, int initial_year)
{
    int index;
    int num_years = 2022 - initial_year + 1;
    for (int y = initial_year; y <= 2022; y++)
    {
        index = y % num_years;
        if (!isEmpty(&years_data[index]))
        {
            // El toString se encarga de calcular los porcentajes de plataforma y el promedio de precios
            printf("%s", toString(&years_data[index]));
        }
    }
}

/*
    Entradas:
        -int *processed_lines: Numero de lineas procesadas por cada hebra
        -int num_threads: numero de hebras generadas.
    Salidas:
        -void
    Descripcion:
        -Imprime la cantidad de lineas procesadas por cada hebra por consola.
*/
void printThreadsProcessedLines(int *processed_lines, int num_threads)
{
    for (int i = 0; i < num_threads; i++)
    {
        printf("Hebra %d: %d lineas procesadas\n", i + 1, processed_lines[i]);
    }
}

// Funciones hebra hija:

/*
    Entradas:
        - char *game_data: linea de datos de un juego.
    Salidas:
        - int year: anio del juego.
    Descripcion:
        - Funcion encargada de obtener el anio de un juego.
*/
int getYear(char *game_data)
{
    char game_data_copy[400];
    strcpy(game_data_copy, game_data);
    int year;
    char *value = strtok(game_data_copy, ",");
    for (int column = 0; column < 5; column++)
    {
        value = strtok(NULL, ",");
    }

    year = atoi(value);
    return year;
}

/*
    Entradas:
        -char *game_data: informacion referente al juego.
        -YearData *year_data: puntero a estructura year data con la informacion referente
            al anio del juego a trabajar y actualizar.
        -float min_price: Precio minimo a considerar.
        -int initial_year: Anio a partir del cual se parte como base hasta el anio actual
    Salidas:
        -void
    Descripcion: Funcion encargada de actualizar la informacion de un YearData
        dada un string (game_data) con informacion referente a un juego.
*/
void updateYearData(char *game_data, YearData *year_data, float min_price, int initial_year)
{
    int year, in_windows = 0, in_mac = 0, in_linux = 0, is_free = 0;
    float price;
    char game_name[100];

    int column = 0;
    char *value = strtok(game_data, ",");
    while (value)
    {
        switch (column)
        {
        case 1:
            strcpy(game_name, value);
            break;
        case 3:
            price = atof(value);
            break;
        case 5:
            year = atoi(value);
            break;
        case 6:
            if (strcmp("True", value) == 0)
            {
                is_free = 1;
            }
            break;
        case 7:
            if (strcmp("Yes", value) == 0)
            {
                in_windows = 1;
            }
            break;
        case 8:
            if (strcmp("Yes", value) == 0)
            {
                in_mac = 1;
            }
            break;
        case 9:
            if (value[0] == 'Y')
            {
                in_linux = 1;
            }
            break;
        default:
            break;
        }

        value = strtok(NULL, ",");
        column++;
    }

    if (initial_year <= year && (min_price <= price || is_free))
    {
        year_data->total_games++;
        year_data->sum_prices += price;
        if (is_free)
        {
            strcpy(year_data->free_games[year_data->num_free_games], game_name);
            year_data->num_free_games++;
        }

        if (in_windows)
        {
            year_data->windows_games++;
        }

        if (in_mac)
        {
            year_data->mac_games++;
        }

        if (in_linux)
        {
            year_data->linux_games++;
        }

        if (price > year_data->price_expensive_game)
        {
            year_data->price_expensive_game = price;
            strcpy(year_data->expensive_game, game_name);
        }
        if (price < year_data->price_cheap_game)
        {
            year_data->price_cheap_game = price;
            strcpy(year_data->cheap_game, game_name);
        }
    }
}