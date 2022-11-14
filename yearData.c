#include "yearData.h"

/*
    Entradas:
        -No posee.
    Salidas:
        -YearData: Estructura YearData inicializada
    Descripcion: 
        -Crea una estructura YearData, con la informacion inicial
        establecida.
*/
YearData *createYearData()
{
    YearData *new_year_data = malloc(sizeof(YearData));
    new_year_data->total_games = 0;
    new_year_data->sum_prices = 0;
    new_year_data->windows_games = 0;
    new_year_data->mac_games = 0;
    new_year_data->linux_games = 0;
    new_year_data->num_free_games = 0;
    return new_year_data;
}

/*
    Entradas:
        -YearData *info: Estructura YearData
    Salidas:
        -char String: String con la informacion de la estructura
    Descripcion: Transforma los datos del año seleccionado en un string    
*/
char *toString(YearData *info)
{
    char *string = malloc(sizeof(char)*1000); 
    char year[50], price_expensive_game[150], price_cheap_game[150], total_games[50], mean[50], total_windows[50], total_mac[50], total_linux[50];

    // conversion de datos a string
    sprintf(year, "%d", info->year);
    sprintf(price_expensive_game, "%.2f", info->price_expensive_game);
    sprintf(price_cheap_game, "%.2f", info->price_cheap_game);
    sprintf(total_games, "%d", info->total_games);
    sprintf(mean, "%.3f", meanPrices(info));
    sprintf(total_windows, "%.2f", getPercentage(info->total_games, info->windows_games));
    sprintf(total_mac, "%.2f", getPercentage(info->total_games, info->mac_games));
    sprintf(total_linux, "%.2f", getPercentage(info->total_games, info->linux_games));
    // concatenacion de datos
    strcpy(string, "");
    strcat(string, "Año: ");
    strcat(string, year);
    strcat(string, "\nJuego mas caro: ");
    strcat(string, info->expensive_game);
    strcat(string, " ");
    strcat(string, price_expensive_game);
    strcat(string, "\nJuego mas barato: ");
    strcat(string, info->cheap_game);
    strcat(string, " ");
    strcat(string, price_cheap_game);
    strcat(string, "\nTotal de juegos: ");
    strcat(string, total_games);
    strcat(string, "\nPromedio de precios: ");
    strcat(string, mean);
    strcat(string, "\nWindows: ");
    strcat(string, total_windows);
    strcat(string, "%");
    strcat(string, "  Mac: ");
    strcat(string, total_mac);
    strcat(string, "%");
    strcat(string, "  Linux: ");
    strcat(string, total_linux);
    strcat(string, "%\n");
    if (info->num_free_games == 0)
    {
        strcat(string, "No hay juegos gratis\n");
    }
    else
    {
        strcat(string, "Juegos gratis:\n");
    }

    for (int i = 0; i < info->num_free_games; i++)
    {
        strcat(string, info->free_games[i]);
        strcat(string, "\n");
    }

    return string;
}

/*
    Entradas::
        -YearData *info : Estructura con los datos del año seleccionado
    Retorno:
        float mean: Promedio de precios de los juegos del año seleccionado
    Descripcion:
        -Calcula el promedio de precios de los juegos del año seleccionado
*/
float meanPrices(YearData *info)
{
    float total = info->total_games;
    float sumPrices = info->sum_prices;
    float mean = sumPrices / total;
    return mean;
}

/*
    Entradas:
        -int total_games: Total de juegos del año seleccionado
        -int plataform_games: Total de juegos de una plataforma en especifico
    Salidas:
        -float: Porcentaje de juegos de una plataforma en especifico
    Descripcion: 
        -Calcula el porcentaje de juegos de una plataforma en especifico
*/
float getPercentage(int total_games, int plataform_games)
{
    return (plataform_games / (float)total_games) * 100;
}

/*
    Entradas:
        YearData: Estructura con los datos del año seleccionado
    Salidas:
        0: si la estructura NO esta vacia.
        1: si la estructura esta vacia.
    Descripcion: 
        -Consulta si la estructura esta vacia.
*/
int isEmpty(YearData *data)
{
    return data->total_games == 0;
}