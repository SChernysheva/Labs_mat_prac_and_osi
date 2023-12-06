#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include "laba2_5_flags.h"

int overfprintf(FILE *file, const char *format, ...)
{
    int countArgs = 0;
    va_list args;
    va_start(args, format);
    int i = 0;
    while (format[i] != '\0')
    {
        if (format[i] != '%')
        {
            fprintf(file, "%c", format[i]);
            i++;
        }
        else
        {
            int g;
            if (format[i + 1] == '%')
            {
                fprintf(file, "%c", '%');
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

                if (flag[1] == 'R' && flag[2] == 'o')
                {
                    int arg = va_arg(args, int);
                    printRomanInFile(file, arg);
                }
                else if (flag[1] == 'C' && tolower(flag[2]) == 'v')
                {
                    int arg = va_arg(args, int);
                    int ss = va_arg(args, int);
                    int isUpper = 1;
                    if (flag[1] == 'v')
                    {
                        isUpper = 0;
                    }
                    printNumBaseInFile(file, arg, ss, isUpper);
                }
                else if (tolower(flag[1]) == 't' && tolower(flag[2]) == 'o')
                {
                    char *num = va_arg(args, char *);
                    int base = va_arg(args, int);
                    fprintf(file, "%d", myInt(num, base));
                }
                else if (flag[1] == 'm')
                {
                    if (flag[2] == 'i')
                    {
                        int arg = va_arg(args, int);
                        printf_dump_memory_file(file, &arg, sizeof(arg));
                    }
                    else if (flag[2] == 'u')
                    {
                        unsigned int arg = va_arg(args, uint);
                        printf_dump_memory_file(file, &arg, sizeof(arg));
                    }
                    else if (flag[2] == 'f')
                    {
                        float arg = va_arg(args, double);
                        printf_dump_memory_file(file, &arg, sizeof(arg));
                    }
                    else if (flag[2] == 'd')
                    {
                        double arg = va_arg(args, double);
                        printf_dump_memory_file(file, &arg, sizeof(arg));
                    }
                }
                else if (flag[1] == 'Z' && flag[2] == 'r')
                {
                    unsigned int arg = va_arg(args, uint);
                    cekendor_file(file, arg);
                }
                else
                {
                    int m = 0;
                    int isN = 0;
                    while (flag[m])
                    {
                        if (flag[m] == 'n')
                        {
                            isN = 1;
                        }
                        m++;
                    }
                    if (isN)
                    {
                        int *arg = va_arg(args, int *);
                        *arg = ftell(file);
                    }
                    else
                    {
                        int n = vfprintf(file, flag, args);
                    } 
                }
            }
            i = g;
        }
    }
    countArgs = ftell(file) - 1;
    return countArgs;
}

int oversprintf(char *buf, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int indexBuf = 0;
    int i = 0;
    while (format[i] != '\0')
    {
        if (format[i] != '%')
        {
            buf[indexBuf] = format[i];
            indexBuf++;
            i++;
        }
        else
        {
            int g;
            if (format[i + 1] == '%')
            {
                sprintf(&(buf[indexBuf]), "%c", '%');
                g = i + 2;
                indexBuf++;
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
                    flag[k] = format[g];
                    g++;
                    k++;
                }
                flag[k] = '\0';
                if (flag[1] == 'R' && flag[2] == 'o')
                {
                    int arg = va_arg(args, int);
                    printRomanInMas(&buf, &indexBuf, arg);
                }
                else if (flag[1] == 'C' && tolower(flag[2]) == 'v')
                {
                    int arg = va_arg(args, int);
                    int ss = va_arg(args, int);
                    int isUpper = 1;
                    if (flag[1] == 'v')
                    {
                        isUpper = 0;
                    }
                    printNumBaseInMas(&buf, &indexBuf, arg, ss, isUpper);
                }
                else if (tolower(flag[1]) == 't' && tolower(flag[2]) == 'o')
                {
                    char *num = va_arg(args, char *);
                    int base = va_arg(args, int);
                    int intNum = myInt(num, base);
                    char res[20];
                    fromIntToChar(intNum, res);
                    int l = 0;
                    while (res[l] != '\0')
                    {
                        buf[indexBuf] = res[l];
                        l++;
                        indexBuf++;
                    }
                }
                else if (flag[1] == 'm')
                {
                    if (flag[2] == 'i')
                    {
                        int arg = va_arg(args, int);
                        print_memory_dump_mas(&arg, sizeof(arg), buf, &indexBuf);
                    }
                    else if (flag[2] == 'u')
                    {
                        unsigned int arg = va_arg(args, uint);
                        print_memory_dump_mas(&arg, sizeof(arg), buf, &indexBuf);
                    }
                    else if (flag[2] == 'f')
                    {
                        float arg = va_arg(args, double);
                        print_memory_dump_mas(&arg, sizeof(arg), buf, &indexBuf);
                    }
                    else if (flag[2] == 'd')
                    {
                        double arg = va_arg(args, double);
                        print_memory_dump_mas(&arg, sizeof(arg), buf, &indexBuf);
                    }
                }
                else if (flag[1] == 'Z' && flag[2] == 'r')
                {
                    unsigned int arg = va_arg(args, uint);
                    cekendor_mas(arg, buf, &indexBuf);
                }
                else
                {
                    int m = 0;
                    int isN = 0;
                    while (flag[m])
                    {
                        if (flag[m] == 'n')
                        {
                            isN = 1;
                        }
                        m++;
                    }
                    if (!isN)
                    {
                        int n = vsprintf(&(buf[indexBuf]), flag, args);
                        indexBuf +=n;
                    } 
                    else
                    {
                        int *arg = va_arg(args, int*);
                        *arg = indexBuf;
                    }
                }
            }
            i = g;
        }
    }
    buf[indexBuf] = '\0';
    return indexBuf;
}