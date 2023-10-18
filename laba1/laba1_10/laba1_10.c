#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define SIZE 33

int checkBaseAndNum(int base, char* num){
    int i = 0;
    if (num[0] == '-') {i = 1;}
    if (base >= 10){
        while(num[i]){
            if (!isalnum(num[i]) ||num[i] - 'A' + 10 >= base) {
                return -1;
            }
            i++;
        }
    }
    else {
        while(num[i]){
            if (num[i] - '0' >= base) {
                return -1;
            }
            i++;
        }
    }
    return 1;
}

int toInt(char* num){
    int minus = 0;
    char* ptr = num;
    if (num[0] == '-') {
        minus = 1;
        ptr++; //!
    }
    while(*ptr == '0') {ptr++;}
    int base = 10;
    int res = 0;
    while(*ptr){
        res = res * base + (isdigit(*ptr) ? *ptr - '0' : *ptr - 'A' + 10);
        ptr++;
    }
    return (minus == 1) ? -res : res;
}

void fromIntTo(int base, int num, char* res, int* len){
    int length = 0;
    int r = 0;
    int i = 0;
    while (num > 0){
        r = num % base;
        res[i] = (r > 9) ? r - 10 + 'A' : r + '0';
        i++;
        length++;
        num/=base;
    }
    *len = length;
    res[i] = '\0';
}

char* getStr(){

    char *mas = (char*)malloc(sizeof(char) * 256);
    if (!mas) {
        return NULL;
    }
    //char mas[256];
    char c = getc(stdin);
    int i = 0;
    while (c != ' ' && c != '\n' && c != '\t' && c != EOF) {
        if (!isalpha(c) && !isdigit(c) && c != '-') {return NULL;}
        //printf("%c\n", c);
        mas[i] = c;
        i++;
        c = getc(stdin);
        if (i > 254) {
            free(mas);
            return NULL;
        }
    }
    if (i == 0) {
        free(mas);
        return NULL;
    }
    mas[i] = '\0';

    return mas;
}


int main(int argc, char*argv[])
{
    int base;
    char* c;
    int mValue = 0;
    printf("Введите систему счисления.\n");
    if ((c = getStr()) != NULL) {
        base = atoi(c);
        if (base < 2 || base > 36 ) {
            printf("Неверный формат ввода, система счисления должна быть числом, не может быть отрицательной или равной нулю.\n");
            free(c);
            return 0;
        }
        free(c);
    }
    else {
        printf("Система счисления не была введена или введен неверный формат.\n");
        return 0;
    }
    
    while (( c = getStr()) != NULL) {
        if (strcmp(c, "Stop") == 0 || strcmp(c, "stop") == 0) {
            free(c);
            break;
        }
        else {
            if (checkBaseAndNum(base, c) == -1) {
                printf("Число не соотвествует введенной сс. Используйте верхний регистр для обозначения букв\n");
                free(c);
                return 0;
            }
            int value = toInt(c);
            free(c);
            int temp = abs(value);
            if ( temp > mValue) {mValue = temp;}
        }
    }
    if (c == NULL) {
        printf("Неверный формат ввода или вы забыли ввести команду стоп.\n");
        return 0;
    }
    char res[256];
    int len = 0;
    printf("Max value by abs is %d\n", mValue);

    //9
    fromIntTo(9, mValue, res, &len);
    printf("Max value in 9: ");
    for (int i = len; i--; i >= 0){
        printf("%c", res[i]);
    }
    printf("\n");

    //18
    fromIntTo(18, mValue, res, &len);
    printf("Max value in 18: ");
    for (int i = len; i--; i >= 0){
        printf("%c", res[i]);
    }
    printf("\n");

    //27
    fromIntTo(27, mValue, res, &len);
    printf("Max value in 27: ");
    for (int i = len; i--; i >= 0){
        printf("%c", res[i]);
    }
    printf("\n");

    //36
    fromIntTo(36, mValue, res, &len);
    printf("Max value in 36: ");
    for (int i = len; i--; i >= 0){
        printf("%c", res[i]);
    }
    printf("\n");
    




    printf("%d", '-');
    





    return 0;
}