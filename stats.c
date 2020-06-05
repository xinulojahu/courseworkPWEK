#include "language.h"
#include "practice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ST_PRINT                                   \
    fprintf(stats_file,                            \
            "%03d|%02d:%02d|%04d|%03d|%05.1f%%\n", \
            num,                                   \
            seconds / 60,                          \
            seconds % 60,                          \
            sym_per_minuts,                        \
            errors,                                \
            errors_prcnt);

int stats_fprint(
        int seconds, int sym_per_minuts, int errors, double errors_prcnt)
{
    FILE* stats_file;
    char language[3];
    get_language(language);
    char* path = malloc(32 * sizeof(char));
    sprintf(path, "%s/results.txt", language);
    stats_file = fopen(path, "r+t");
    if (stats_file == NULL) {
        stats_file = fopen(path, "w+t");
    }
    free(path);

    FILE* temp_file;
    temp_file = fopen("res_temp.txt", "w+");

    fpos_t pos;
    int num = 1;
    int best_spm = 0;
    char* str;
    str = malloc(64 * sizeof(char));

    if (fgets(str, 64, stats_file) != NULL) {
        fgetpos(stats_file, &pos);
        char* cur = str;
        for (int i = 0; i < 2; i++) {
            while (*cur != '|') {
                cur++;
            }
            cur++;
        }
        best_spm = char_to_int(cur, '|');
        fgets(str, 64, stats_file);
        num = (char_to_int(str, '|') + 1) % 1000;
        fputs(str, temp_file);
    }
    while (fgets(str, 64, stats_file) != NULL) {
        fputs(str, temp_file);
    }
    if (best_spm < sym_per_minuts) {
        fseek(stats_file, 0, SEEK_SET);
        ST_PRINT;
    }
    fsetpos(stats_file, &pos);
    ST_PRINT;
    fseek(temp_file, 0, SEEK_SET);
    while (fgets(str, 64, temp_file) != NULL) {
        fputs(str, stats_file);
    }
    remove("res_temp.txt");
    fclose(temp_file);
    fclose(stats_file);
    free(str);
    return 0;
}

int stats_export()
{
    FILE* stats_file;
    char language[3];
    get_language(language);
    char* path = malloc(32 * sizeof(char));
    sprintf(path, "%s/results.txt", language);
    stats_file = fopen(path, "r");
    if (stats_file == NULL) {
        printf("Результатов еще нет");
        return -1;
    }
    free(path);
    char* str;
    str = malloc(64 * sizeof(char));

    fgets(str, 64, stats_file);
    FILE* ploterror;
    FILE* plotspm;
    ploterror = fopen("gnuplot_error.txt", "w");
    plotspm = fopen("gnuplot_spm.txt", "w");

    int spm;
    int num;
    double error;
    char* cur;
    while (fgets(str, 64, stats_file) != NULL) {
        num = char_to_int(str, '|');
        cur = str;
        for (int i = 0; i < 2; i++) {
            while (*cur != '|') {
                cur++;
            }
            cur++;
        }
        spm = char_to_int(cur, '|');
        for (int i = 0; i < 2; i++) {
            while (*cur != '|') {
                cur++;
            }
            cur++;
        }
        error = char_to_int(cur, ',');
        while (*cur != ',') {
            cur++;
        }
        cur++;
        error += char_to_int(cur, '%') * 0.1;
        fprintf(plotspm, "%d %d\n", num, spm);
        fprintf(ploterror, "%d %.1lf\n", num, error);
    }
    fclose(ploterror);
    fclose(plotspm);
    fclose(stats_file);
    free(str);
    return 0;
}

int stats(int num)
{
    FILE* stats_file;
    char language[3];
    get_language(language);
    char* path = malloc(32 * sizeof(char));
    sprintf(path, "%s/results.txt", language);
    stats_file = fopen(path, "r+t");
    if (stats_file == NULL) {
        stats_file = fopen(path, "w+t");
    }
    free(path);

    char* estr;
    char* str;
    str = malloc(100 * sizeof(char));
    printf("Argument is %d\n", num); //Вспомогательная строка
    if (stats_file == NULL) { //Проверка на открытие файла
        printf("Error openning the file!\n");
        return -1;
    }
    //Чтение построчно данных из файла в бесконечном цикле
    while (1) {
        // Чтение одной строки  из файла
        estr = fgets(str, 100, stats_file);
        //Проверка на конец файла или конец аргумента
        if ((estr == NULL) || (num == 0)) {
            if (feof(stats_file) != 0) { //Остановка цикла при конце файла
                printf("Reading is complete!\n");
                break;
            } else {
                printf("Reading is complete.\n");
                break;
            }
        }
        puts(str);     //Вывод строки
        num = num - 1; //Счетчик
    }
    //Закрытие файла
    free(str);
    fclose(stats_file);
    printf("\n");
    return 0;
}
