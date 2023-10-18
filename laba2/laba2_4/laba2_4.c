#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
typedef struct point
{
    double x;
    double y;
} point;

typedef struct line{
    double x;
    double y;
} line;

int isConvex(int n, ...){
    va_list(args);
    va_start(args, n);
    point* points = (point*)malloc(sizeof(point) * n);
    int index = 0;
    int coord = 0;
    for (int i = 0; i < n * 2; i++){
        if (coord == 0){
            points[index].x = va_arg(args, double);
            coord = 1;
        }
        else {
            points[index].y = va_arg(args, double);
            coord = 0;
            index++;
        }
    }
    va_end(args);
    line* lines = (line*)malloc(sizeof(line) * n);
    for (int i = 0; i < n - 1; i++){
        lines[i].x = points[i + 1].x - points[i].x;
        lines[i].y = points[i + 1].y - points[i].y;
    }
    lines[n - 1].x = points[0].x - points[n - 1].x;
    lines[n - 1].y = points[0].y - points[n - 1].y;
    int direct = 0;
    int temp = 0;
    for (int i = 0; i < n - 1; i++){
        temp = lines[i].x * lines[i + 1].y - lines[i + 1].x * lines[i].y;
        if (direct == 0){
            if (temp < 0) {direct = -1;}
            else { direct = 1;}
        }
        else {
            if (temp > 0 && direct < 0){
                return 0;
            }
            if (temp < 0 && direct > 0){
                return 0;
            }
        }
    }
    temp = lines[n - 1].x * lines[0].y - lines[0].x * lines[n - 1].y;
    if (direct == 0){
        if (temp < 0) {direct = -1;}
        else { direct = 1;}
    }
    else {
        if (temp > 0 && direct < 0){
            return 0;
        }
        if (temp < 0 && direct > 0){
            return 0;
        }
    }
    if (direct == 0) {return 0;}
    return 1;
}

double foo2(double x, int n, ...)
{
    va_list coeff;
    va_start(coeff, n);

    double result = 0.0;
    double power = 1.0;

    for (int i = 0; i <= n; i++) {
        double coefficient = va_arg(coeff, double); //2 4 
        //printf("%f\n", coefficient);
        result += coefficient * power;
        power *= x;
    }

    va_end(coeff);

    return result;
}

int main(){
    printf("%f\n", foo2(1.0, 2, 2.0, 1.0, 3.0)); //2*1 + 1*1 + 3
    printf("%d\n", isConvex(5, -1, -1, 1, 4, 3, 0, 1, -1, -2, 2));
    return 0;
}