#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
double f(double x) {
    return 4 - 2 * x * x;
}
double get_solution(double a, double b, double eps, double (*f)(double)){
    double c;
    while(b - a > eps){
        c = (a + b) / 2;
        if (f(b) * f(c) < 0){ a = c; }
        else {b = c;}
    }
    return (a + b) / 2;
}


int main(){

    printf("%f\n", get_solution(0, 2, 0.02, f));
    return 0;
}