#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include "laba2_6_flags.h"

int oversscanf(char *buf, const char *format, ...);
int overfscanf(FILE *file, const char *format, ...);

int overfscanf(FILE *file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int countArgs = 0;
    int i = 0;
    int my_seek = 0;
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            int g;
            if (format[i + 1] == '%')
            {
                char c;
                fscanf(file, "%c", &c);
                my_seek += 3;
                g = i + 2;
            }
            else
            {
                char flag[25];
                int k = 0;
                g = i;
                flag[k] = format[g];
                k++;
                g++;
                while (format[g] != '\0' && (isalnum(format[g]) || format[g] == '.'))
                {
                    char c = format[g];
                    flag[k] = c;
                    g++;
                    k++;
                }
                flag[k] = '\0';
                char nextSym = format[g];
                if (nextSym == '%')
                    nextSym = ' ';
                if (flag[1] == 'R' && flag[2] == 'o') ////M C D X L V I
                {
                    fseek(file, my_seek, SEEK_SET);
                    int *arg = va_arg(args, int *);
                    int j = 0;
                    char romNum[256];
                    char c = fgetc(file);
                    my_seek++;
                    if (isRoman(c))
                    {
                        j++;
                    }
                    else
                    {
                        while (!isRoman(c) && c != EOF)
                        {
                            c = fgetc(file);
                            my_seek++;
                        }
                    }
                    while (c != EOF && isRoman(c))
                    {
                        romNum[j] = c;
                        j++;
                        c = fgetc(file);
                        my_seek++;
                    }
                    if (j != 0)
                    {
                        romNum[j] = '\0';
                        *arg = romanToInt(romNum);
                        countArgs++;
                    }
                }
                else if (flag[1] == 'C' && tolower(flag[2]) == 'v') // overfprintf(file, "%Cv", int &num, int ss)
                {
                    fseek(file, my_seek, SEEK_SET);
                    int *arg = va_arg(args, int *);
                    int ss = va_arg(args, int);
                    if (ss < 2 || ss > 26)
                    {
                        ss = 10;
                    }
                    int j = 0;
                    char num[256];
                    char c = fgetc(file);
                    my_seek++;
                    if (!isalnum(c))
                    {
                        my_seek--;
                        while (!isalnum(c) && c != EOF)
                        {
                            c = fgetc(file);
                            my_seek++;
                        }
                    }
                    while (c != EOF && isalnum(c))
                    {
                        num[j] = c;
                        j++;
                        c = fgetc(file);
                        my_seek++;
                    }
                    if (j != 0)
                    {
                        num[j] = '\0';
                        *arg = myInt(num, ss);
                        countArgs++;
                    }
                }
                else if (flag[1] == 'Z' && flag[2] == 'r')
                {
                    fseek(file, my_seek, SEEK_SET);
                    unsigned int *arg = va_arg(args, uint *);
                    char num[256];
                    int j = 0;
                    char c = fgetc(file);
                    my_seek++;
                    if (c != '0' || c != '1')
                    {
                        my_seek--;
                        while (c != '0' && c != '1' && c != EOF)
                        {
                            c = fgetc(file);
                            my_seek++;
                        }
                    }
                    while (c != EOF && (c == '0' || c == '1') && j <= 256)
                    {
                        num[j] = c;
                        c = fgetc(file);
                        my_seek++;
                        j++;
                    }
                    if (j != 0)
                    {
                        num[j] = '\0';
                        *arg = get_num_cekend(num);
                        countArgs++;
                    }
                }
                else
                {
                    fseek(file, my_seek, SEEK_SET);
                    int n = vfscanf(file, flag, args);
                    void* arg = va_arg(args, void*);
                    my_seek = ftell(file);

                }
            }
            i = g;
        }
        else
        {
            i++;
        }
    }
    return countArgs;
}

int oversscanf(char *buf, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int countArgs = 0;
    int i = 0;
    int index_buf = 0;
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            int g;
            if (format[i + 1] == '%')
            {
                char c;
                sscanf(&(buf[index_buf]), "%c",  &c);
                g = i + 2;
                countArgs++;
                index_buf += 3;
            }
            else
            {
                char flag[25];
                int k = 0;
                g = i;
                flag[k] = format[g];
                k++;
                g++;
                while (format[g] != '\0' && (isalnum(format[g]) || format[g] == '.'))
                {
                    char c = format[g];
                    flag[k] = c;
                    g++;
                    k++;
                }
                flag[k] = '\0';
                char nextSym = format[g];
                if (nextSym == '%')
                    nextSym = ' ';
                if (flag[1] == 'R' && flag[2] == 'o') ////M C D X L V I
                {
                    int *arg = va_arg(args, int *);
                    int j = 0;
                    char romNum[256];
                    char c = buf[index_buf];
                    if (!isRoman(c))
                    {
                        index_buf++;
                        while (!isRoman(buf[index_buf]) && c != EOF)
                        {
                            index_buf++;
                        }
                    }
                    c = buf[index_buf];
                    while (c != EOF && isRoman(c))
                    {
                        romNum[j] = c;
                        j++;
                        index_buf++;
                        c = buf[index_buf];
                    }
                    if (j != 0)
                    {
                        romNum[j] = '\0';
                        *arg = romanToInt(romNum);
                        countArgs++;
                    }
                }                                                   ////////////////////
                else if (flag[1] == 'C' && tolower(flag[2]) == 'v') // overfprintf(file, "%Cv", int &num, int ss)
                {
                    int *arg = va_arg(args, int *);
                    int ss = va_arg(args, int);
                    if (ss < 2 || ss > 26)
                    {
                        ss = 10;
                    }
                    int j = 0;
                    char num[256];
                    char c = buf[index_buf];
                    index_buf++;
                    if (!isalnum(c))
                    {
                        index_buf--;
                        while (!isalnum(c) && c != EOF)
                        {
                            c = buf[index_buf];
                            index_buf++;
                        }
                    }
                    while (c != EOF && isalnum(c))
                    {
                        num[j] = c;
                        j++;
                        c = buf[index_buf];
                        index_buf++;
                    }
                    if (j != 0)
                    {
                        num[j] = '\0';
                        *arg = myInt(num, ss);
                        countArgs++;
                    }
                }
                else if (flag[1] == 'Z' && flag[2] == 'r')
                {
                    unsigned int *arg = va_arg(args, uint *);
                    char num[256];
                    int j = 0;
                    char c = buf[index_buf];
                    index_buf++;
                    if (c != '0' && c != '1')
                    {
                        while (c != '0' && c != '1' && c != EOF)
                        {
                            c = buf[index_buf];
                            index_buf++;
                        }
                    }
                    while (c != EOF && (c == '0' || c == '1') && j <= 256)
                    {
                        num[j] = c;
                        c = buf[index_buf];
                        index_buf++;
                        j++;
                    }
                    if (j != 0)
                    {
                        num[j] = '\0';
                        *arg = get_num_cekend(num);
                        countArgs++;
                    }
                }
                else
                {
                    while (isspace(buf[index_buf]))
                    {
                        index_buf++;
                    }
                    vsscanf(&(buf[index_buf]), flag, args);
                    void* arg = va_arg(args, void*);
                    while (buf[index_buf] != EOF && buf[index_buf] != nextSym && !(isspace(buf[index_buf])))
                    {
                        index_buf++;
                    }
                }
            }
            i = g;
        }
        else
        {
            i++;
        }
    }
    return countArgs;
}