#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define SIZE 10

enum checkArgs1 {
    OK,
    WRONG_SYMB,
    OVERFLOW
};

enum checkArgs1 Check(char* str){
    int i = 0;
    if (str[i] == '-') i = 1;
    while(str[i]){
        if (i == 10) {return OVERFLOW;}
        if(!isdigit(str[i])) {return WRONG_SYMB;}
        i++;
    }
    return OK;
} 

void findMaxMin(int* mas, int* min, int* max, int len){
    int minValue = INT_MAX;
    int maxValue = -INT_MAX;
    int minIndex, maxIndex;
    for (int i = 0; i < len; i++){
        if (mas[i] > maxValue) {
            maxValue = mas[i];
            maxIndex = i;
        }
        else if (mas[i] < minValue) {
            minValue = mas[i];
            minIndex = i;
        }
    }
    min[0] = minValue;
    max[0] = maxValue;
    min[1] = minIndex;
    max[1] = maxIndex;
}

void ChangeElems(int* mas, int i1, int i2){
    int temp = mas[i1];
    mas[i1] = mas[i2];
    mas[i2] = temp;
}

void FillingMas(int* mas, int len, int a, int b){
    for (int i = 0; i < len; i++){
        mas[i] = rand() % (b - a + 1) + a;
    }
}

void printMas(int* mas, int len){
    for (int i = 0; i < len; i++){
        printf("%d ", mas[i]);
    }
    printf("\n");
}

void BubbleSort(int* mas, int len){
    for (int i = 0; i < len; i++){
        for (int j = i; j < len; ++j){
            if (mas[i] > mas[j]){
                ChangeElems(mas, i, j);
            }
        }
    }
}

int findClosest(int* mas, int num, int len){
    int diff = INT_MAX;
    int ind = len/2;
    if (num > mas[ind]) {
        ind++;
        while(num > mas[ind] && ind < len){
            ind++;
        }
        return ((num - mas[ind]) <= (num - mas[ind-1])) ? mas[ind] : mas[ind - 1];

    }
    else if (num < mas[ind]){
        ind--;
        while(num < mas[ind] && ind > 0){
            ind--;
        }
        return (abs(num - mas[ind]) <= abs(num - mas[ind+1])) ? mas[ind] : mas[ind + 1];
    }
    else {return mas[ind];}
}

int main(int argc, char*argv[])
{
    if (argc == 3)
    {
        if (Check(argv[1]) != OK || Check(argv[2]) != OK){
            printf("Неверный формат ввода или слишком большое/маленькое число, введите число инт\n");
            return 0;
        }
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        if (a > b) {
            printf("Неверный формат ввода, первое число - минимум, второе - максимум\n");
            return 0;
        }
        int mas[SIZE];
        FillingMas(mas, SIZE, a, b);
        int max[2], min[2];

        printf("Your mas:\n");
        printMas(mas, SIZE);

        findMaxMin(mas, min, max, SIZE);

        printf("Max = %d, min = %d\n", max[0], min[0]);

        ChangeElems(mas, max[1], min[1]);
        
        printf("Your mas after changing:\n");
        printMas(mas, SIZE);

        return 0;
    }
    else if(argc == 1){

        int sizeA = rand() % (10000 - 10 + 1) + 10;
        int sizeB = rand() % (10000 - 10 + 1) + 10;

        int* masA = (int*)malloc(sizeof(int) * sizeA);
        if (!masA) {
            printf("Не удалось выделить память.\n");
            return 0;
        }

        int* masB = (int*)malloc(sizeof(int) * sizeB);
        if (!masB) {
            printf("Не удалось выделить память.\n");
            free(masA);
            return 0;
        }

        FillingMas(masA, sizeA, -1000, 1000);
        FillingMas(masB, sizeB, -1000, 1000);

        //printf("Массив А:\n");
        //printMas(masA, sizeA);
        //printf("Массив B:\n");
        //printMas(masB, sizeB);

        int* masC = (int*)malloc(sizeof(int) * sizeA);
        if (!masC) {
            printf("Не удалось выделить память.\n");
            free(masA);
            free(masB);
            return 0;
        }

        BubbleSort(masB, sizeB);

        for (int i = 0; i < sizeA; i++){
            masC[i] = masA[i] + findClosest(masB, masA[i], sizeB);
        }

        printMas(masC, sizeA);
    }

    return 0;
}