#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

enum baseAnswer{
    overflow,
    wrongSymb,
    OK
}; 

enum baseAnswer findMinBase(char* mas, int *base){
    int i = 0;
    if (mas[0] == '-') i = 1;
    int maxValue = 0;
    while(mas[i]){
        if (i >= 10) {
            return overflow;
        }
        if (!isalnum(mas[i])) {
            return wrongSymb;
        }
        if (isalpha(mas[i])) mas[i] = toupper(mas[i]);
        if((int)mas[i] > maxValue) maxValue = (int)mas[i];
        i++;
    }
    *base = (maxValue <= 57) ? maxValue - '0' + 1 : maxValue - 'A' + 11;
    return OK;
}

int toInt(char* num) {
    int minus = 0;
    char* ptr = num;
    if (num[0] == '-') {
        minus = 1;
        ptr++; 
    }
    while(*ptr == '0') {ptr++;}
    int base = 10;
    int res = 0;
    while(*ptr){
        res = res * base + (isdigit(*ptr) ? *ptr - '0' : toupper(*ptr) - 'A' + 10);
        ptr++;
    }
    return (minus == 1) ? -res : res;
}

void FillingFail(FILE* in, FILE* out){
    char c = fgetc(in);
    while (c != EOF)
    {
        char mas[256];
        int i = 0;
        while (c != ' ' && c != '\n' && c != '\t' && c != EOF && c > 33) {
            mas[i] = c;
            i++;
            c = fgetc(in);
        }
        if (i == 0) {
            c = fgetc(in);
            continue; 
        }
        mas[i] = '\0';
        int base = 0;
        int m = 0;
        while(mas[m]){
            fputc(mas[m], out);
            m++;
        }
        enum baseAnswer ans = findMinBase(mas, &base);
        fputc(' ', out);
        if (ans == OK) {
        fprintf(out, "%d", base);
        fputc(' ', out);
        fprintf(out, "%d", toInt(mas));
        fputc('\n', out);
        }
        else {
            char* error = "Overflow or wrong symb";
            fprintf(out, "%s", error);
            fputc('\n', out);
        }
        if (c == EOF) return;
        c = fgetc(in);
    }
}

int main(int argc, char*argv[])
{
    if (argc != 3 ){
        printf("Недостаточное кол-во аргументов или неверный ввод\n");
        return 0;
    }
    FILE* in;
    FILE* out;
    if(!(in = fopen(argv[1], "r")))
    {
        printf("Не удалось открыть входной файл.\n");
        return 0;
    }

    if(!(out = fopen(argv[2], "w"))) 
    {
        printf("Не удалось создать/открыть файл результата.\n");
        fclose(in);
        return 0;
    }

    FillingFail(in, out);

    fclose(in);
    fclose(out);

    return 0;
}