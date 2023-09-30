#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>


int flagA(FILE* in, FILE* out){
    char c;
    while ((c = fgetc(in)) != EOF)
        {
            if (c > 33 && (c < '0' || c > '9')) 
            {
                fprintf(out, "%X", c);
            }
            else fputc(c, out);
        }
    if(c == EOF)
    {
        if (feof(in) == 0)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 1;
}

int flagS(FILE* in, FILE* out){
    char c;
    int count = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == '\n') 
        {
            fputc(count+'0', out);
            fputc(c, out);
            count = 0;
        }
        if ( (c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || ( c>= 123 && c <= 176)) {count++;} 
    }
    if(c == EOF)
    {
        if (feof(in) == 0)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 1;
}

int flagI(FILE* in, FILE* out){
    char c;
    int count = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if (c == '\n') 
        {
            fputc(count+'0', out);
            fputc(c, out);
            count=0;
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {count++;}
    }
    if(c == EOF)
    {
        if (feof(in) == 0)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 1;
}


int flagD(FILE* in, FILE* out){
    char c;
    while ((c = fgetc(in)) != EOF)
    {
            if (c < '0' || c > '9')
            {
                fputc(c, out);
            }
    }
    if (c == EOF){
        if (feof(in) == 0)
        {
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 1;
}

int main(int argc, char*argv[])
{
    if( ((argc!=4) && (argc!=3)) || ((strlen(argv[1])!=2) && strlen(argv[1])!=3) || ((argv[1][0]!='-') && (argv[1][0]!='/')))
    {
            printf("Неправильный формат ввода. Первый аргумент-флаг, затем аргументы для него.\n");
            printf("Символ перед флагом: - или /\n");
            return 0;
    } //проверка на корректность ввода
    FILE *in;
    FILE *out;
    int checkN = 0;
    if(!(in = fopen(argv[2], "r")))
    {
        printf("Не удалось открыть входной файл.\n");
        return 0;
    }
    if (argv[1][1] == 'n')
    {
        checkN=1;
        if (((argv[1][2] != 'd') && (argv[1][2] != 'i') && (argv[1][2] != 's') && (argv[1][2] != 'a')) || argc != 4)
        {
            printf("Неправильный формат ввода. Первый аргумент-флаг, затем аргументы для него.\n");
            printf("Возможные флаги: -d, -i, -s, -a \n");
            fclose(in);
            return 0;
        }
        if(!(out = fopen(argv[3], "w"))) 
        {
            printf("Не удалось создать файл результата.\n");
            fclose(in);
            return 0;
        }
    }
    else 
    {
        if (((argv[1][1] != 'd') && (argv[1][1] != 'i') && (argv[1][1] != 's') && (argv[1][1] != 'a')) || argc != 3)
        {
            printf("Неправильный формат ввода. Первый аргумент-флаг, затем аргументы для него.\n");
            printf("Возможные флаги: -d, -i, -s, -a \n");
            return 0;
        }
        char *fout=(char*)malloc((size_t)strlen("_out") + (size_t)(strlen(argv[2]) + (size_t)strlen(".txt")+1));
        if (!fout) {
            printf("Не удалось создать файл результата.\n");
            fclose(in);
            return 0;
        }
        memcpy(fout, argv[2], strlen(argv[2]));
        memcpy(fout+strlen(argv[2]), "_out.txt", 8);
        fout[8+strlen(argv[2])] = '\0';
        if(!(out = fopen(fout, "w"))) 
        {
            printf("Не удалось создать файл результата1.\n");
            fclose(in);
            return 0;
        }
        free(fout);
    }

    int indexF = 1;
    if(checkN == 1) {indexF = 2;}
    
    if(argv[1][indexF] == 'd')
    {
        if (flagD(in, out) == -1) {
            printf("Ошибка чтения из файла\n");
            return 0;
        }
    }

    if(argv[1][indexF] == 'i')
    {
        if (flagI(in, out) == -1) {
            printf("Ошибка чтения из файла\n");
            return 0;
        }
    }

    if(argv[1][indexF] =='s')
    {
        if (flagS(in, out) == -1) {
            printf("Ошибка чтения из файла\n");
            return 0;
        }
    }

    if(argv[1][indexF] == 'a')
    {
        if (flagA(in, out) == -1) {
            printf("Ошибка чтения из файла\n");
            return 0;
        }
    }

return 0;
}