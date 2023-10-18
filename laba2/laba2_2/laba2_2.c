#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

double foo1(int count, ...)
{
    double res = 1;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        res *= va_arg(args, double);
    }
    va_end(args);
    return pow(res, 1.0 / count);
}

double foo2(double num, int degree)  //5 -2   res=1/5 1/25 
{
    if (degree == 0)
    {
        return 1;
    }
    if (degree == 1)
    {
        return num;
    }
    if (degree == -1){
        return 1 / num;
    }
    if (degree < 0) {
        double res = foo2(num, degree / 2);
        res *= res;
        if (abs(degree % 2) == 1){
            res /= num;
        }
        return res;
    }
    double half = foo2(num, degree / 2);
    if (degree % 2 == 0)
    {
        return half * half;
    }
    else
    {
        return half * half * num;
    }
}

int main(int argc, char *argv[])
{

    // check foo1
    int count = 4;
    printf("Res for foo1 if count = %d\n", count);
    printf("%f\n", foo1(5, 2.0, 3.0, 4.0, 5.0, 6.0));

    // check foo2
    int degree = 10;
    double num = 1.1;
    printf("Res for foo2 if base = %f and degree = %d\n", num, degree);
    printf("%f\n", foo2(num, degree));
}