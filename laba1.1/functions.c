#include "functions.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>



void flagH(int num, int result[], int sizeOfMas) {
    int c=num;
    int i=0;
    while(c<=100){
        result[i]=c;
        i++;
        c+=num;
    };
} 

int flagP(int num) {
    //0-простое 1-составное
    for (int i=2; i<num; i++){
        if (num%i==0){
            return 1;
        }
    }
    return 0;
}

long long int flagA(int num){
    if(num<0) {
        return -1;
    }
    long long int res=((num+1)*num)/2;
    return res;
}

void flagS(const char*s, int count, char res[]){
    int i=0; 
    for (i; i<=count; i++){
        res[i]=s[i]; //посимвольно добавляем в массив ответов размером равным длине заданного числа символы
    }
}

void flagE(int stepen, float mas[][stepen]){
    for (int i=0; i<11; i++){
            for (int j=0; j<stepen; j++){
                mas[i][j]=pow((double)i, (double)j);
            }
    }
}

int fact(int num) {
    if (num>17) return -1;
    return (num<2) ? 1 : num*fact(num-1);
}

int isInInt(const char*s){
    if (strlen(s)>10) return 0;
    if (strlen(s)==10 && s[0]!='1' && s[0]!='2') return 0;
    if (strlen(s)==10 && s[0]=='2' && s[1]!='1' && s[1]!='0') return 0;
    if (strlen(s)==10 && s[0]=='2' && s[1]=='1'){
        for (int i=2; i<=9; i++){
            if (s[i]!='0') return 0;
        }
    }
    return 1;
}


int Atoi(const char*s){ //Ручная реализация функции атои + проверка на переполнение
    if (isInInt(s)==0) return 0;
    int res=0;
    int i=0;
    if(s[0]=='-') i=1; // если число отрицательно - пропускаем минус, добавим в конце
    while(s[i]){
        res = res*10 +(s[i] - '0');
        i++;
    }
    return (s[0]!='-') ? res : -res;
}

int isInt(const char*s){
    int i = 0;
    if (s[0]=='-')  i=1;
    while(s[i]){
        if (s[i] != '0' && s[i] != '1' && s[i] != '2' && s[i] != '3' && s[i] != '4' && s[i] != '5' && s[i] != '6' && s[i] != '7' && s[i] != '8' && s[i] != '9') return 0;
        i++;
    }
    return 1;
}