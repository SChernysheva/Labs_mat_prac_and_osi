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
int find_NOD(int a, int b)
{
    while (b != 0)
    {
        int c = a % b;
        a = b;
        b = c;
    }
    return a;
}
int isPrime(int num)
{//0 - простое 1- составное
    if (num==1 || num==0) return -1;
    if (num==2) return 0;
    if (num%2==0) return 1;
    int sqr=sqrt(num);

    for (int i=3; i<=sqr; i+=2){
        if (num%i==0){
            return 1;
        }
    }
    return 0;
}
enum ans check_prime_devisors(int denominator, int base) //base% den
{
    for (int i = 2; i <= denominator / 2; i++)
    {
        if (denominator % i == 0 && isPrime(i) == 0)
        {
            //printf("%d %d %d\n", denominator, i, base);
            if (base % i != 0) 
            {
                return No;
            }
        }
    }
    return Yes;
}
enum ans checkNum(double num, int base)
{ 
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
    int nod = find_NOD(numerator, denominator);
    numerator /= nod;
    denominator /= nod;
    //printf("%f %d %d\n", num, numerator, denominator);
    return check_prime_devisors(denominator, base);
}
enum memory_check foo(struct res **mas, int ss, int n, ...) // 0 1 2  3
{
    *mas = malloc(sizeof(struct res) * (n + 1));
    if (!(*mas))
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
    int base = 8;
    enum memory_check res = foo(&mas, base, 3, 0.5, 0.33, 0.1);
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