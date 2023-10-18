#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
enum checkMemory
{
    OK,
    ERROR
};
struct checker
{
    int count; // если все ок то кол-во обработанных файлов если не ок то номер файла который не смогли открыть
    int isOk;  // 1 если все ок и -1 если какой то файл не открылся
};
struct answer
{ // для вывода ответов номера строк и символа в ней
    int sym;
    int str;
};
int findLen(char *str)
{
    int len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return len;
}
int FindLenOfFile(FILE *in)
{
    int i = 0;
    char c;
    while ((c = fgetc(in)) != EOF)
    {
        i++;
    }
    return i;
}
enum checkMemory findStr(FILE *in, char *str, struct answer **ans)
{
    int lenOfFile = FindLenOfFile(in);
    int len = findLen(str);
    *ans = malloc(sizeof(struct answer) * (lenOfFile / len)); // максимальное кол-во возможных ответов кол-во символов в файле делить на длину строки
    if (!ans)
    {
        return ERROR;
    }
    int indexOfMas = 0;
    int numOfStr = 1;
    int numOfSymb = 1;
    fseek(in, 0, SEEK_SET);
    char c = fgetc(in);

    while (c != EOF)
    {
        if (c == '\n')
        {
            numOfStr++;
            numOfSymb = 0;
        }
        if (str[0] == c)
        {
            int start = numOfSymb;
            int startStr = numOfStr;
            c = fgetc(in);
            numOfSymb++;
            if (c == '\n')
            {
                numOfStr++;
                numOfSymb = 0;
            }
            if (c == EOF)
            {
                break;
            }
            int i = 1;
            while (c == str[i] && str[i])
            {
                c = fgetc(in);
                numOfSymb++;
                i++;
                if (c == '\n')
                {
                    numOfStr++;
                    numOfSymb = 0;
                }
                if (c == EOF)
                {
                    break;
                }
            }
            if (i == len)
            {
                (*ans)[indexOfMas].sym = start;
                (*ans)[indexOfMas].str = startStr;
                indexOfMas++;
            }
        }
        c = fgetc(in);
        numOfSymb++;
    }
    (*ans)[indexOfMas].str = '\0';
    (*ans)[indexOfMas].sym = '\0';
    return OK;
}

struct checker checkFiles(struct answer *res[10], char *str, int count, ...)
{
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        FILE *in;
        if (!(in = fopen(va_arg(args, char *), "r")))
        {
            struct checker error;
            error.count = i;
            error.isOk = -1;
            return error;
        };
        struct answer *mas;
        if (findStr(in, str, &mas) != OK)
        {
            struct checker error;
            error.count = i;
            error.isOk = -1;
            return error;
        }
        res[i] = mas;
        fclose(in);
    }
    va_end(args);
    struct checker OK;
    OK.count = count;
    OK.isOk = 1;
    return OK;
}
int main()
{
    struct answer *res[10];
    struct checker ans = checkFiles(res, "\n", 2, "test.txt", "test2.txt");
    if (ans.isOk == -1)
    {
        printf("Не удалось открыть один из переданных файлов или не удалось выделить память под один из них\n");
        int i = 0;
        while (i < ans.count)
        {
            free(res[i]);
        }
        return 1;
    }
    int i = 0;
    int m = 0;
    while (i < ans.count)
    {
        printf("Номера строки и символов этих строк для файла %d\n", i + 1);
        while ((res[i][m]).str)
        {
            printf("%d %d\n", (res[i][m]).str, (res[i][m]).sym);
            m++;
        }
        free(res[i]);
        i++;
        m = 0;
    }
    return 0;
}