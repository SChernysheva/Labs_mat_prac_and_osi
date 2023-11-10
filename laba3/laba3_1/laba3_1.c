#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};

int check_zero(int num)
{
    return (num | 0) ? 1 : 0;
}
int get_len(int num, int pow_2)
{
    if (num == 0) return 1;
    int count = 0;
    while (check_zero(num))
    {
        num = num >> pow_2;
        count++;
    }
    return count;
}
int my_plus(int a, int b)
{
    while (b)
    {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}
int my_minus(int a, int b)
{
    int carry;
    while (check_zero(b))
    {
        carry = (~a) & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}
int my_mult(int a, int b)
{
    int res = 0;
    while(check_zero(b))
    {
        if (b & 0x1 == 0x1)
        {
            res = my_plus(res, a);
        }
        b >>= 1;
        a <<= 1;
    }
    return res;
}
int my_abs(int a)
{
    int mask = a >> (my_minus(my_mult(sizeof(int), 8), 1));
    return my_plus(a, mask) ^ mask;
}
int my_remainder(int num, int divisor)
{
    return num & my_minus(divisor, 1);
}
int whole_part(int num, int pow_2)
{
    return num >> pow_2;
}
enum checker get_num_ss(int num, int ss, int pow_2, char** res)
{
    int checkMinus = 0;
    if (num < 0) 
    {
        checkMinus = 1;
        num = my_abs(num);
    }
    int len = my_plus(get_len(num, pow_2), checkMinus);
    *res = (char*)malloc( my_mult(sizeof(char), len));
    if (!(*res))
    {
        return ERROR_MEMORY;
    }
    if (num == 0)
    {
        (*res)[0] = '0';
        (*res)[1] = '\0';
        return OK;
    }
    int i = my_minus(len, 1);
    while (check_zero(num))
    {
        int remaind = my_remainder(num, ss);
        (*res)[i] = remaind >= 10 ? my_plus(remaind, 55) : my_plus(remaind, '0');
        //printf("%d %c\n", my_remainder(num, ss), (*res)[i]);
        num = whole_part(num, pow_2);
        i = my_minus(i, 1);
    }
    if (checkMinus == 1) (*res)[0] = '-';
    (*res)[len] = '\0';
    return OK;
}

enum checker foo(char** res, int num)
{
    if (res == NULL)
    {
        return INVALID;
    }
    int k = 1;
    for (int i = 2; i <= 32; i = my_mult(i, 2))
    {
        enum checker ans;
        ans = get_num_ss(num, i, k, &(*res));
        if (ans != OK)
        {
           for (int m = 0; m <= k; m = my_plus(m, 1))
            {
                free(*res);
            }
            return ans; 
        }
        res++;
        k = my_plus(k, 1);
    }
    return OK;
}
int main() //почистить память валидация обработка ошибок
{
    //printf("%d", my_mult(5, 2));
    char* res[5];
    int num = -5;
    enum checker ans = foo(res, num);
    if (ans == ERROR_MEMORY)
    {
        printf("Ошибка с выделением памяти\n");
        return 0;
    }
    if (ans == INVALID)
    {
        printf("Некорректные аргументы\n");
        return 0;
    }
    printf("Число %d в системе счисления 2 в степени:\n1 %s\n2 %s\n3 %s\n4 %s\n5 %s\n", num, res[0], res[1], res[2], res[3], res[4]);
    for (int i = 0; i < 5; i++)
    {
        free(res[i]);
    }
    return 0;
}