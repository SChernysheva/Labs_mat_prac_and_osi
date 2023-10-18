#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

int main(int argc, char*argv[]){
    if (argc != 2){
        printf("Введите название файла\n");
        return 1;
    }

    FILE* in;
    FILE* out;
    if (!(in = fopen(argv[1], "rb"))){
        printf("Не удалось открыть файл\n");
        return 0;
    }

    int i = 0;
    int lenBeforeDot = 0;
    char* format = (char*)malloc(sizeof(char) * 6);
    while(argv[1][i] != '.'){
        i++;
    }
    lenBeforeDot = i;
    int m = 0;
    if (argv[1][i] == '.'){
        while(argv[1][i]){
            format[m] = argv[1][i];
            i++;
            m++;
        }
    }
    format[m] = '\0';

    int len = lenBeforeDot + strlen("_copy") + strlen(format) + 1; 
    char *fout = (char*)malloc(sizeof(char) * len);
    if (!fout){
        printf("Недостаточнр памяти\n");
        free(format);
        fclose(in);
        fclose(out);
        return 0;
    }
    memcpy(fout, argv[1], lenBeforeDot);
    memcpy(fout + lenBeforeDot, "_copy", strlen("_copy"));
    memcpy(fout + lenBeforeDot + strlen("_copy"), format, strlen(format));
    fout[lenBeforeDot + strlen("copy") + strlen(format) + 1] = '\0';
    free(format);

    if ( !(out = fopen(fout, "wb"))){
        printf("Не удалось создать файл результата1.\n");
            fclose(in);
            free(fout);
            return 0;
    }
    free(fout);
    fseek(in, 0, SEEK_END);
    long size = ftell(in);

    char* buff = (char*)malloc(sizeof(char) * size);
    if (!buff){
        printf("Не удалось выделить память.\n");
        fclose(in);
        fclose(out);
        return 0;
    }

    fseek(in, 0, SEEK_SET);
    fread(buff, sizeof(char), size, in);
    fwrite(buff, sizeof(char), size, out);

    free(buff);
    fclose(in);
    fclose(out);
    return 0;
}