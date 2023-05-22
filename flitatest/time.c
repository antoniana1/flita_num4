#include "time.h"

void GiveRandFile1(int numberOfNumbers) {
    FILE *fp;
    if ((fp = fopen("..\\forgraph.txt", "w")) == NULL) {
        puts("Can't open file first_graph.txt");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL)); // Функция чтоб rand() каждый раз генерировал новые числа
    for (int i = 0; i < numberOfNumbers - 1; i++) { // Генерируем и записываем числа в файл
        for (int j = 0; j < numberOfNumbers - 1; j++) {
            fprintf(fp, "%d %d\n", 1 + rand() % DENOMINATOR, 1 + rand() % DENOMINATOR);
        }
        fprintf(fp, "%d %d\n", 1 + rand() % DENOMINATOR, 1 + rand() % DENOMINATOR);
        fclose(fp);
    }
}


