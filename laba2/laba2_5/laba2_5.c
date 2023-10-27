#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "laba2_5_funcs.h"

int main()
{
    FILE* file = fopen("test.txt", "w");
    if (!file)
    {
        printf("Не удалось открыть файл\n");
    }
    int x;
    overfprintf(file, "I want%%%d %n %.00000001f", 55, &x, 0.10000001);
    char mas[150];
    oversprintf(mas, "I want%%%d", 55);
    printf("%d\n", x);
    printf("%s", mas);
    printf("\n");
    fclose(file);
    return 0;
}

