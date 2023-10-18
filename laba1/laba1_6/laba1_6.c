#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
enum checkArgs{
    OK,
    ERROR
};
enum checkArgs checkStr(char* str){
    int len = strlen(str);
    if (len > 10 || len == 0){
        return ERROR;
    }
    if (*str == '-' || *str == '.'){
       return ERROR;
    }
    int point = 0;
    while(*str){
        if (*str == '.'){
            if (point != 0){
                return ERROR;
            }
            point = 1;
        }
        else if (!isdigit(*str) && ((*str) != 'e')){
                return ERROR;
        }
        str++;
    }
}
double task1(double x){
    return log(1 + x) / x;
}
double task2(double x){
    return exp( - (pow(x, 2) / 2));
}
double task3(double x){
    if (x == 1) return -1;
    return log(1 / (1 - x));
}
double task4(double x){
    return pow(x, x);
}

double calucalateByRightRect(double start, double end, double count, double (*f)(double), double eps){
    double h = (end - start) / count;
    double s1 = 0.0;
    double s2 = eps + 0.1;
    while (fabs(s2 - s1) > eps){
        s1 = s2;
        s2 = 0.0;
        for (int i = 1; i <= count; i++){
            double x = start + i * h;
            double ans = f(x);
            s2 += f(x);
        }
    }
    return h * s2;
}

int main(int argc, char*argv[])
{
    if (argc != 2){
        printf("Недостаточно аргументов\n");
        return 1;
    }
    enum checkArgs ans = checkStr(argv[1]);
    if (ans != OK){
        printf("Неверный формат ввода.\n");
        return 1;
    }
    double eps = atof(argv[1]);
    if (eps == 0){
        printf("Эпсилон не может быть равен нулю\n");
        return 1;
    }
    printf("1 task res = %lf\n", calucalateByRightRect(0, 1, 100, task1, eps));
    printf("2 task res = %lf\n", calucalateByRightRect(0, 1, 100, task2, eps));
    printf("3 task res = %lf\n", calucalateByRightRect(0, 1, 100, task3, eps));
    printf("4 task res = %lf\n", calucalateByRightRect(0, 1, 100, task4, eps));

    return 0;
}