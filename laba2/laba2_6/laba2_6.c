#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include "laba2_6_funcs.h"

int main()
{
    FILE* file;
    if (!(file = fopen("test.txt", "r")))
    {
        printf("Не удалось открыть файл\n");
        return 0;
    }
    int rim;
    int num;
    float s;
    int ss = 16;
    int res;
    int num1;
    int num2;
    char str[10];
    char mas[150];
    //sprintf(mas, "%s", "255 XL %% 0.1000001 allala");
    overfscanf(file, "%d %Ro %Zr %% %f %s", &num, &num2, &num1, &s, str);
    printf("%d %d %d %f %s\n", num, num2, num1, s, str);
    fclose(file);
}
