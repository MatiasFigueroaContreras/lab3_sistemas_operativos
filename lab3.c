// Borrar repetidos ocupados en funciones.h y yearData.h
#include "funciones.h"
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

// Datos Globales:
//  Datos Compartidos
FILE *input_file;     // Archivo de entrada
YearData *years_data; // Estructura con la informacion de los anios
pthread_mutex_t *mutexs_year_data;
pthread_mutex_t mutex_file;
int *threads_processed_lines;

/*
    Entradas:
        - FILE *fp: Salida por consola
        - const char *path: Ruta del archivo
    Salidas:
        - void
    Descripcion:
        -Imprime la informacion de uso referente al formato
        correcto para la ejecucion del programa.
*/
void usage(FILE *fp, const char *path)
{
    const char *basename = path + 2;
    fprintf(fp, "usage: %s [OPTION]\n", basename);
    fprintf(fp, "The following is the correct format for using the program:\t\t"
                "\n'./lab3 -i input_file -o output_file -d year -p min_price -n num_threads -c chunk -b'\n");
    fprintf(fp, "  -h, --help\t\t"
                "Print this help and exit.\n");
    fprintf(fp, "  -i, --input[=INPUTFILENAME]\t"
                "File to read games data.\n");
    fprintf(fp, "  -o, --output[=OUTPUTFILENAME]\t"
                "File to write the output data generated.\n");
    fprintf(fp, "  -d, --year[=YEAR]\t"
                "year required to start the search.\n");
    fprintf(fp, "  -p, --min[=MINPRICE]\t"
                "Minimum price required to start the search.\n");
    fprintf(fp, "  -n, --threads[=NUMTHREADS]\t"
                "Number of threads to create.\n");
    fprintf(fp, "  -c, --chunk[=CHUNK]\t"
                "Number of lines to read for chunk.\n");
    fprintf(fp, "  -b, --print[=PRINTFLAG]\t"
                "print the data in the console.\n");
}

typedef struct
{
    int tid;
    int chunk;
    int initial_year;
    float min_price;
} t_data;

void *daughterThread(void *arg)
{
    t_data *data = (t_data *)arg;
    char games_data[data->chunk][400];
    int year, index, num_games;
    int num_years = 2022 - data->initial_year + 1;
    int eof = 0;
    while (!eof)
    {
        num_games = 0;
        pthread_mutex_lock(&mutex_file);
        for (int i = 0; i < data->chunk && !eof; i++)
        {
            if (fgets(games_data[i], 400, input_file))
            {
                threads_processed_lines[data->tid]++;
                num_games++;
            }
            else
            {
                eof = 1;
            }
        }
        pthread_mutex_unlock(&mutex_file);

        for (int i = 0; i < num_games; i++)
        {
            year = getYear(games_data[i]);
            index = year % num_years;
            pthread_mutex_lock(&mutexs_year_data[index]);
            updateYearData(games_data[i], &years_data[index], data->min_price, data->initial_year);
            pthread_mutex_unlock(&mutexs_year_data[index]);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    char s_num_threads[1000], s_year[6], s_min_price[10], s_chunk[6];
    int print_flag = 0;
    int help_flag = 0;
    int num_threads = 0;
    int chunk = 0;
    int opt, initial_year = -1;
    float min_price = -1;
    char name_input_file[100], name_output_file[100];

    struct option longopts[] =
        {{"help", no_argument, &help_flag, 1},
         {"input", required_argument, NULL, 'i'},
         {"output", required_argument, NULL, 'o'},
         {"year", required_argument, NULL, 'd'},
         {"min", required_argument, NULL, 'p'},
         {"threads", required_argument, NULL, 'n'},
         {"chunk", required_argument, NULL, 'c'},
         {"print", optional_argument, NULL, 'b'}};

    while ((opt = getopt_long(argc, argv, ":i:o:d:p:n:c:b::h", longopts, 0)) != -1)
    {
        switch (opt)
        {
        case 'i': // Archivo de entrada
            strcpy(name_input_file, optarg);
            break;
        case 'o': // Archivo de salida
            strcpy(name_output_file, optarg);
            break;
        case 'd': // Año de inicio juego
            strcpy(s_year, optarg);
            initial_year = atoi(s_year);
            break;
        case 'p': // Precio minimo
            strcpy(s_min_price, optarg);
            min_price = atof(s_min_price);
            break;
        case 'n': // Numero de hebras a crear
            strcpy(s_num_threads, optarg);
            num_threads = atoi(s_num_threads);
            break;
        case 'c': // Numero de hebras a crear
            strcpy(s_chunk, optarg);
            chunk = atoi(s_chunk);
            break;
        case 'b': // flag para imprimir los datos por pantalla
            print_flag = 1;
            break;
        case 'h':
            usage(stdout, argv[0]);
            return 0;
        case '?': // flag que no existe
            usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        case ':': // cuando se usa mal una flag
            usage(stderr, argv[0]);
            exit(EXIT_FAILURE);
        default: // se aborta cuando falta flag o argumento obligatorio
            abort();
            break;
        }
    }

    if (name_input_file == NULL || name_output_file == NULL || initial_year < 0 || min_price < 0 || num_threads <= 0 || help_flag || chunk <= 0)
    {
        // Entra si no se ingreso alguna opcion o valor necesario para
        //   el funcionamiento del programa, si se ingresa un valor no
        //   valido, o si se ingresa la flag de ayuda.
        usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    if (access(name_input_file, 0) != 0)
    {
        // Entra si el archivo de entrada no existe.
        perror("Input file doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    // Inicializacion de datos compartidos:
    input_file = fopen(name_input_file, "r");
    years_data = createYearsDataArray(initial_year);
    threads_processed_lines = malloc(sizeof(int) * num_threads);

    // Inicializacion de mutexs:
    int num_years = 2022 - initial_year + 1;
    mutexs_year_data = malloc(sizeof(pthread_mutex_t) * num_years);
    for (int i = 0; i < num_years; i++)
    {
        pthread_mutex_init(&mutexs_year_data[i], NULL);
    }

    pthread_mutex_init(&mutex_file, NULL);

    // Creacion de hebras:
    pthread_t tid[num_threads];
    t_data data;
    data.chunk = chunk;
    data.initial_year = initial_year;
    data.min_price = min_price;

    for (int i = 0; i < num_threads; i++)
    {
        data.tid = i;
        threads_processed_lines[i] = 0;
        pthread_create(&tid[i], NULL, &daughterThread, (void *)&data);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }

    // Salidas del programa:
    fclose(input_file);
    writeOutputFile(name_output_file, years_data, initial_year);
    if (print_flag)
    {
        printYearsData(years_data, initial_year);
        printThreadsProcessedLines(threads_processed_lines, num_threads);
    }
    
    return 0;
}
