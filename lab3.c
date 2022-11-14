// Borrar repetidos ocupados en funciones.h y yearData.h
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

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
                "\n'./lab2 -i input_file -o output_file -d year -p min_price -n num_workers -b'\n");
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
    fprintf(fp, "  -n, --threads[=NUMWORKERS]\t"
                "Number of threads to create.\n");
    fprintf(fp, "  -c, --chunk[=NUMWORKERS]\t"
                "Number of lines to read for chunk.\n");
    fprintf(fp, "  -b, --print[=PRINTFLAG]\t"
                "print the data in the console.\n");
}

int main(int argc, char *argv[])
{
    char s_num_threads[1000], s_year[6], s_min_price[10], s_chunk;
    char s_print_flag = '0';
    int help_flag = 0;
    int num_threads = 0;
    int chunk = 0;
    int opt, year = -1;
    float min_price = -1;
    char input_file[100], output_file[100];

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
            strcpy(input_file, optarg);
            break;
        case 'o': // Archivo de salida
            strcpy(output_file, optarg);
            break;
        case 'd': // Año de inicio juego
            strcpy(s_year, optarg);
            year = atoi(s_year);
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
            s_print_flag = '1';
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

    if (input_file == NULL || output_file == NULL || year < 0 || min_price < 0 || num_threads <= 0 || help_flag || chunk <= 0)
    {
        // Entra si no se ingreso alguna opcion o valor necesario para
        //   el funcionamiento del programa, ademas si se ingresa
        //   la flag de ayuda.
        usage(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    if (access(input_file, 0) != 0)
    {
        // Entra si el archivo de entrada no existe
        perror("Input file doesn't exist\n");
        exit(EXIT_FAILURE);
    }

    
}