#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
struct Vector
{
    double* coord;
}typedef vector;
enum checker 
{
    OK,
    ERROR,
    INVALID
};
enum checker get_max_len(double* res, vector vec, int dimension)
{
    if (dimension <= 0) return INVALID;
    double max = vec.coord[0];
    for (int i = 1; i < dimension; i++)
    {
        if (vec.coord[i] > max) max = vec.coord[i];
    }
    *res = max;
    return OK;
}
enum checker chebyshev_norm(double* res, vector vec, int dimension, int p)
{
    if (p < 1 || dimension <= 0) return INVALID;
    double sum = 0;
    for (int i = 0; i < dimension; i++)
    {
        sum += pow(abs(vec.coord[i]), p);
    }
    *res = pow(sum, 1 / p);
    return OK;
}
enum checker matrix(double* res, vector vec, int dimension)
{
    if (dimension <= 0) return INVALID;
    int matrix[dimension][dimension];
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < dimension; i++)
    {
        matrix[i][i] = 1;
    }
    double res_[dimension];
    for (int i = 0; i < dimension; i++)
    {
        double sum;
        for (int j = 0; j < dimension; j++)
        {
            sum += matrix[i][j] * vec.coord[j];
        }
        res_[i] = sum;
    }
    double res__ = 0;
    for (int i = 0; i < dimension; i++)
    {
        res__ += pow(res_[i], 2);
    }
    *res = pow(res__, 0.5);
    //printf("%f\n", *res);
    return OK;
}
enum checker my_foo(double* res, vector* res_v, int dimension, int p, enum checker (*foo1)(double*, vector, int), enum checker (*foo2)(double*, vector, int, int), enum checker (*foo3)(double*, vector, int), int count, ...)
{
    //*size = count;
    va_list(vecs);
    va_start(vecs, count);
    double maxs[3];
    maxs[0] = INT_MIN;
    maxs[1]= INT_MIN;
    maxs[2] = INT_MIN;
    for (int i = 0; i < count; i++) //векторы
    {
        vector vec = va_arg(vecs, vector); //берем каждый веткор
        double res_[3];  //ищем для него ответы
        if (foo1(&(res_[0]), vec, dimension) != OK) return ERROR;
        if (foo2(&(res_[1]), vec, dimension, p) != OK) return ERROR; 
        if (foo3(&(res_[2]), vec, dimension) != OK) return ERROR;
        for (int m = 0; m < 3; m++) //сравниваем каждый ответ с нашим максимумом
        {
            if (res_[m] > maxs[m]) 
            {
                maxs[m] = res_[m];  
                res_v[m] = vec; //вектор 
                res[m] = maxs[m]; //само число(ответ)
            }
        }
    }
    return OK;
}
void print_vec(vector vec, int dimension)
{
    for (int i = 0; i < dimension; i++)
    {
        printf("%f ", vec.coord[i]);
    }
    printf("\n");
}
int main()
{
    int p = 1;
    int dimension = 3;

    vector vec1;
    vec1.coord = (double*)malloc(sizeof(double) * dimension);
    vec1.coord[0] = 0.1;
    vec1.coord[1] = 0.5;
    vec1.coord[2] = 3.2;

    vector vec2;
    vec2.coord = (double*)malloc(sizeof(double) * dimension);
    vec2.coord[0] = 0.1;
    vec2.coord[1] = 2.5;
    vec2.coord[2] = 3.2;

    vector vec3;
    vec3.coord = (double*)malloc(sizeof(double) * dimension);
    vec3.coord[0] = 0.5;
    vec3.coord[1] = 2.5;
    vec3.coord[2] = 3.0;

    double res[3];   
    vector res_v[3];                   
    int count;
    my_foo(res, res_v, dimension, p, get_max_len, chebyshev_norm, matrix, 3, vec1, vec2, vec3);

    for (int i = 0; i < 3; i++)
    {
        printf("Для нормы номер %d самый длинный переданный вектор:\n", i + 1);
        print_vec(res_v[i], dimension);
        printf("и его значение %lf\n", res[i]);
    }
    free(vec1.coord);
    free(vec2.coord);
    
    return 0;
}