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
    file = fopen("test.txt", "r");
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
    //sscanf(mas, "%d %f %s", &num, &s, str);
    overfscanf(file, "%d %Ro %Zr %% %f %s", &num, &num2, &num1, &s, str);
    //printf("%d\n", overfscanf(file, "%Ro %d:%f:%Cv %Cv %Zr %s", &rim,  &num, &s, &res, 16, &num1, 37, &num2, str));
    printf("%d %d %d %f %s\n", num, num2, num1, s, str);
}