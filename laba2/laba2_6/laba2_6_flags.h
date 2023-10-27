#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

int get_len(int num);
int get_num_cekend(char *sum);
int isRoman(char c);
void fromIntToChar(int num, char *res);
int myInt(char *str, int base);
void fromIntTo(int base, int num, char *res, int *len);
int romanToInt(char *roman);

int finddLen(int num)
{
    int len = 0;
    while(num != 0)
    {
        num /= 10;
        len++;
    }
    return len;
}
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
int get_num_cekend(char *sum)
{
    int res = 0;
    int k = 0;
    int last = 1;
    int last2 = 1;
    int current_fib = 1;
    while (sum[k] != '\0') // 0001
    {
        if (sum[k] == '1')
        {
            res += current_fib;
        }
        last = last2;
        last2 = current_fib;
        current_fib = last + last2;
        k++;
    }
    return res;
}
int isRoman(char c)
{
    return (c == 'M' || c == 'C' || c == 'D' || c == 'X' || c == 'L' || c == 'V' || c == 'I') ? 1 : 0;
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
int myInt(char *str, int base)
{
    int minus = 0;
    if (str[0] == '-')
    {
        minus = 1;
    }
    int len = strlen(str);
    int res = 0;
    for (int i = minus; i < len; i++)
    {
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
int romanToInt(char *roman)
{
    int res = 0;
    int len = strlen(roman);
    for (int i = 0; i < len; i++)
    {
        switch (roman[i])
        {
        case 'I':
            if (i + 1 < len && (roman[i + 1] == 'V' || roman[i + 1] == 'X'))
            {
                res -= 1;
            }
            else
            {
                res += 1;
            }
            break;
        case 'V':
            res += 5;
            break;
        case 'X':
            if (i + 1 < len && (roman[i + 1] == 'L' || roman[i + 1] == 'C'))
            {
                res -= 10;
            }
            else
            {
                res += 10;
            }
            break;
        case 'C':
            if (i + 1 < len && (roman[i + 1] == 'D' || roman[i + 1] == 'M'))
            {
                res -= 100;
            }
            else
            {
                res += 100;
            }
            break;
        case 'L':
            res += 50;
            break;
        case 'D':
            res += 500;
            break;
        case 'M':
            res += 1000;
            break;
        default:
            break;
        }
    }
    return res;
}

