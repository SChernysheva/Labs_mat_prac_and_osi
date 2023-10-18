#ifndef FOO
#define FOO

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>



void flagH(int num, int result[]);

int flagP(int num);

long long int flagA(int num);

void flagS(int num, char **res, int *flag);

void flagE(int degree, int mas[][degree]);

int fact(int num);

int isInInt(const char*s);

int Atoi(const char*s);

int isInt(const char*s);

#endif 