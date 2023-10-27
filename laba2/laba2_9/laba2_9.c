#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
// 3/ 10 -16 == 4 
enum memory_check
{
    ERROR,
    OK
};
enum ans
{
    Yes,
    No
};
struct res
{
    int ans;
    double num;
};
enum ans checkNum(double num, int base) 
{
    int whole_part = num / 1; 
    double fraction = num - whole_part; 
    double res = fraction * base; 
    int whole_res = res / 1; 
    double fraction_res = res - whole_res; 
    int count = 0;
    int mas[10];
    for (int i = 0; i < 10; i++)
    {
        mas[i] = 0;
    }
    while (fraction_res != 0)
    {
        fraction_res *= base; 
        int whole_part = fraction_res / 1;
        fraction_res = fraction_res - whole_part;
        count++;
        if (mas[(int)(fraction_res * 10)] == 1)
        {
            return No;
        }
        mas[(int)(fraction_res * 10)] = 1;
    }
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
        printf("%f\n", num);
        enum ans ans= checkNum(num, ss);
        (*mas)[i].ans = (ans == Yes) ? 1 : 0;
        (*mas)[i].num = num;
    }
    (*mas)[n].ans = -1;
    va_end(args);
    return OK;
}

int main()
{
    struct res* mas;
    enum memory_check res = foo(&mas, 15, 3, 0.5, 0.2, 0.3);
    if (res == ERROR)
    {
        printf("Не удалось выделить память\n");
    }
    int i = 0;
    while(mas[i].ans != -1)
    {
        printf("Для дроби %f конечное представление ", mas[i].num);
        (mas[i].ans == 1) ? printf("определено\n") : printf("не определено\n");
        i++;
    }
    free(mas);
    return 0;
}
