#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
enum memory_check
{
    ERROR,
    OK
};
enum ans
{
    Yes,
    No,
    Error_memory
};
struct res
{
    int ans;
    double num;
};
enum ans checkNum(double num, int base)
{ // Находим остаток от деления числителя на знаменатель - m % n = 1.
    // Это число у нас будет выступать в качестве начального значения.
    // 1) Сохраняем остаток в массиве. Умножаем остаток (массив не трогаем) на основание системы счисления (у нас десятичная) - 1*10 = 10
    // 2) Находим остаток от деления числа с шага [1] на знаменатель - 10 % 3 = 1
    // 3) Если он равен нулю, то дробь конечная. Иначе ищем остаток в массиве.
    // Если нашли, то мы нашли период. Иначе берём текущий остаток и переходим к шагу [1].
    int numerator, denominator;
    double temp = num;
    numerator = (int)num;
    denominator = 1;
    while (numerator != temp)
    {
        temp *= 10;
        numerator = (int)temp;
        denominator *= 10;
    }
    int fraction = numerator;         // 0.12 числитель
    int ost = fraction % denominator; // Находим остаток от деления числителя на знаменатель - m % n = 1.

    int indexMas = 0;
    int *mas = (int *)malloc(sizeof(int) * 10);
    if (!mas)
    {
        return Error_memory;
    }
    int len = 10;
    mas[indexMas] = ost; // Сохраняем остаток в массиве.
    indexMas++;
    int res = ost * base; // Умножаем остаток (массив не трогаем) на основание системы счисления 1

    int whole_res = res % denominator; // Находим остаток от деления числа с шага [1] на знаменатель - 10 % 3 = 1 2
    while (whole_res != 0)             // 3
    {
        int i = 0;
        while(mas[i])
        {
            if (mas[i] == whole_res)
            {
                free(mas);
                return No;
            }
            i++;
        }
        if (indexMas >= len)
        {
            int *temp = (int *)malloc(sizeof(int) * (len * 2));
            if (!temp)
            {
                free(mas);
                return Error_memory;
            }
            free(mas);
            mas = temp;
            len *= 2;
        }
        mas[indexMas] = whole_res;
        indexMas++;
        mas[indexMas] = '\0';
        whole_res *= base;
        whole_res = whole_res % denominator;
    }
    free(mas);
    return Yes;
}
enum memory_check foo(struct res **mas, int ss, int n, ...)
{
    *mas = malloc(sizeof(struct res) * n);
    if (!mas)
    {
        return ERROR;
    }
    va_list(args);
    va_start(args, n);
    for (int i = 0; i < n; i++)
    {
        double num = va_arg(args, double);
        enum ans ans = checkNum(num, ss);
        if (ans == Error_memory)
        {
            return ERROR;
        }
        (*mas)[i].ans = (ans == Yes) ? 1 : 0;
        (*mas)[i].num = num;
    }
    (*mas)[n].ans = -1;
    va_end(args);
    return OK;
}

int main()
{
    struct res *mas;
    int base = 10;
    enum memory_check res = foo(&mas, base, 3, 2.3455, 0.33, 0.5);
    if (res == ERROR)
    {
        printf("Не удалось выделить память\n");
    }
    int i = 0;
    while (mas[i].ans != -1)
    {
        printf("Для дроби %f конечное представление ", mas[i].num);
        (mas[i].ans == 1) ? printf("определено\n") : printf("не определено\n"); // 2,345 / 10
        i++;
    }
    free(mas);
    return 0;
}