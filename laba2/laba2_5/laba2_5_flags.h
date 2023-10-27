#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

int get_len(int num);
int fib(int n);
int getNumFib(int num);
void cekendor_file(FILE* file, unsigned int num);
void cekendor_mas(unsigned int num, char* buf, int* indexBuf);
void fromIntToChar(int num, char *res);
int myInt(char *num, int base);
void fromIntTo(int base, int num, char *res, int *len);
void intToRoman(int num, char *res);
void printRomanInFile(FILE *file, int num);
void printRomanInMas(char **buf, int *indexBuf, int arg);
void printNumBaseInFile(FILE *file, int num, int base, int isUpper);
void printNumBaseInMas(char **buf, int *indexBuf, int num, int base, int isUpper);
void printf_dump_memory_file(FILE* file, void* num, size_t size);
void print_memory_dump_mas(void* num, size_t size, char* buf, int* indexBuf); 


int get_len(int num)
{
    int len = 0;
    while (num != 0)
    {
        num /= 10;
        len++;
    }
    return len;
}
int fib(int n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;
    else return fib(n - 1) + fib(n - 2);
}
int getNumFib(int num)
{
    int n = 2;
    int last_fib = 0;
    int current_fib = 1;
    while (current_fib < num)
    {
        last_fib = current_fib;
        n++;
        current_fib = fib(n);
    }
    if (current_fib == num){
        return current_fib;
    }
    else if (last_fib <= num) return last_fib;
    else return fib(n - 2);
}
void cekendor_file(FILE* file, unsigned int num)
{
    int my_fibs[5];
    if (num == 0) {
        my_fibs[0] = 0;
        my_fibs[1] = '\0';
    }
    else if (num == 1)
    {
        my_fibs[0] = 1;
        my_fibs[1] = '\0';
    }
    else
    {
        int index_fibs = 0;
        int current_fib = getNumFib(num);
        if (current_fib == num)
        {
            my_fibs[index_fibs] = current_fib;
            my_fibs[index_fibs + 1] = '\0';
        }
        else
        {
            while (current_fib != num)
            {
                my_fibs[index_fibs] = current_fib;
                index_fibs++;
                num = num - current_fib;
                current_fib = getNumFib(num);
            }
            my_fibs[index_fibs] = current_fib;
            my_fibs[index_fibs + 1] = '\0';
        }
    }
    int k = 0;
    int last = 1; //0
    int last2 = 1; //1
    int current_fib = 1;
    int len = sizeof(my_fibs) / sizeof(int);
    while (my_fibs[k] != '\0')
    {
        int res = 0;
        for (int i = 0; i < len; i++)
        {
            if (my_fibs[i] == current_fib)
            {
                fprintf(file, "%d", 1);
                res = 1;
                k++;
            }
        }
        if (res == 0)
        {
            fprintf(file, "%d", 0);
        }
        last = last2; //2
        last2 = current_fib; //3
        current_fib = last + last2; //5
    }
}

void cekendor_mas(unsigned int num, char* buf, int* indexBuf)
{
    int my_fibs[5];
    if (num == 0) {
        my_fibs[0] = 0;
        my_fibs[1] = '\0';
    }
    else if (num == 1)
    {
        my_fibs[0] = 1;
        my_fibs[1] = '\0';
    }
    else
    {
        int index_fibs = 0;
        int current_fib = getNumFib(num);
        if (current_fib == num)
        {
            my_fibs[index_fibs] = current_fib;
            my_fibs[index_fibs + 1] = '\0';
        }
        else
        {
            while (current_fib != num)
            {
                my_fibs[index_fibs] = current_fib;
                index_fibs++;
                num = num - current_fib;
                current_fib = getNumFib(num);
            }
            my_fibs[index_fibs] = current_fib;
            my_fibs[index_fibs + 1] = '\0';
        }
    }
    int k = 0;
    int last = 1; //0
    int last2 = 1; //1
    int current_fib = 1;
    int len = sizeof(my_fibs) / sizeof(int);
    while (my_fibs[k] != '\0')
    {
        int res = 0;
        for (int i = 0; i < len; i++)
        {
            if (my_fibs[i] == current_fib)
            {
                buf[(*indexBuf)++] = '1';
                res = 1;
                k++;
            }
        }
        if (res == 0)
        {
            buf[(*indexBuf)++] = '0';
        }
        last = last2; //2
        last2 = current_fib; //3
        current_fib = last + last2; //5
    }
}
void fromIntToChar(int num, char *res)
{
    int len = 0;
    int temp = num;
    while (temp != 0)
    {
        temp /= 10;
        len++;
    }
    int index = 0;
    if (num < 0)
    {
        res[0] = '-';
        index = 1;
        num = abs(num);
    }
    res[len] = '\0';
    for (int i = len - 1; i >= index; i--)
    {
        res[i] = (num % 10) + '0';
        num /= 10;
    }
}
int myInt(char* str, int base)
{
    int minus = 0;
    if (str[0] == '-')
    {
        minus = 1;
    }
    int len = strlen(str);
    int res = 0;
    for (int i = minus; i < len; i++){
        res += ((isdigit(str[i]) ? str[i] - '0' : toupper(str[i]) - 'A' + 10) * pow(base, len - 1 - i));
    }
    return (minus == 1) ? -res : res;
}
void fromIntTo(int base, int num, char *res, int *len)
{
    int length = 0;
    int r = 0;
    int i = 0;
    while (num > 0)
    {
        r = num % base;
        res[i] = (r > 9) ? r - 10 + 'A' : r + '0';
        i++;
        length++;
        num /= base;
    }
    *len = length;
    res[i] = '\0';
}
void intToRoman(int num, char *res)
{
    int decimal[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}; 
    int i = 0;
    while (num > 0)
    {
        if (num >= decimal[i])
        {
            strcat(res, roman[i]);
            num -= decimal[i];
        }
        else
        {
            i++;
        }
    }
    res[i] = '\0';
}
void printRomanInFile(FILE *file, int num)
{
    char res[20];
    res[0] = '\0';
    if (num <= 0)
    {
        fprintf(file, "%d", num);
    }
    intToRoman(num, res);
    fprintf(file, "%s", res);
}
void printRomanInMas(char **buf, int *indexBuf, int arg)
{
    char res[20];
    if (arg <= 0)
    {
        char res[20];
        fromIntToChar(arg, res);
        int k = 0;
        while (res[k] != '\0')
        {
            if (!(*buf)[*indexBuf])
            {
                return;
            }
            (*buf)[*indexBuf] = res[k];
            k++;
            (*indexBuf)++;
        }
        return;
    }
    else
    {
        res[0] = '\0';
        intToRoman(arg, res);
    }
    int k = 0;
    while (res[k] != '\0')
    {
        (*buf)[*indexBuf] = res[k];
        k++;
        (*indexBuf)++;
    }
}
void printNumBaseInFile(FILE *file, int num, int base, int isUpper)
{
    if (base < 2 || base > 36)
    {
        base = 10;
    }
    char res[20];
    int i = 0;
    int len = 0;
    fromIntTo(base, num, res, &len);
    for (int i = len; i--; i >= 0)
    {
        (isUpper == 0) ? fprintf(file, "%c", tolower(res[i])) : fprintf(file, "%c", res[i]);
    }
}
void printNumBaseInMas(char **buf, int *indexBuf, int num, int base, int isUpper)
{
    if (base < 2 || base > 36)
    {
        base = 10;
    }
    char res[20];
    int i = 0;
    int len = 0;
    fromIntTo(base, num, res, &len);
    for (int i = len; i--; i >= 0)
    {
        (*buf)[*indexBuf] = (isUpper == 0) ? tolower(res[i]) : res[i];
        (*indexBuf)++;
    }
}
void printf_dump_memory_file(FILE* file, void* num, size_t size)
{
    unsigned char *bytes = (unsigned char*)num; 
    for (size_t i = 0; i < size; i++)
    {
        for (int j = sizeof(unsigned char) * 8 - 1; j >= 0; j--)
        {
            fputc( ((bytes[i] >> j) & 1) + '0', file);
        }
        fputc(' ', file);
    }
}


void print_memory_dump_mas(void* num, size_t size, char* buf, int* indexBuf) 
{
    unsigned char *bytes = (unsigned char*)num; 
    for (size_t i = 0; i < size; i++)
    {
        unsigned char byte = bytes[i];
        for (int j = sizeof(unsigned char) * 8 - 1; j >= 0; j--)
        {
            buf[*indexBuf + (i * sizeof(unsigned char) * 8 + j)] = ((byte >> j) & 1) ? '1' : '0';
        }
        buf[*indexBuf + (i * sizeof(unsigned char) * 8 + sizeof(unsigned char) * 8)] = ' ';
        (*indexBuf)++;
    } 
    (*indexBuf) = *indexBuf + 31;
}


