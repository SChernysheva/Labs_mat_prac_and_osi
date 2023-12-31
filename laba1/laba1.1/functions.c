#include "functions.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>



void flagH(int num, int result[]) {
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

long long int flagA(int num){
    if(num<0) {
        return -1;
    }
    long long int res=((num+1)*num)/2; //более быстрая формула для расчета суммы от 1 до числа, используем лонг лонг инт чтобы даже при самом большом возможном инте не было переполнения
    return res;
}

void flagS(int num, char **res, int *flag){ 
    if(n==0) {
        *flag=-1;
        return 0;
    } 
    int temp=num;
    *flag=0;
    while(temp>0){
        (*flag)++; 
        temp/=10;
    }

    *res=(char*)malloc(sizeof(char)*(*flag));
    if (*res==NULL){
        *flag=-1;
        return;
    }

    *flag=0;

    while(num!=0){
        (*res)[*flag] = (char)(num%10);
        (*flag)++;
        num/=10;
    }
}


void flagE(int degree, int mas[][degree]){  
    mas[0][0]=1;
    for (int i=1; i<11; i++) {
        mas[i][0]=1;
        mas[i][1]=i;
    }
    for (int i=2; i<degree; i++) {
        mas[0][i]=0;
        mas[1][i]=1;
    }

    for (int osn=2; osn<11; osn++){
        for (int st=2; st<degree; st++){
            mas[osn][st]=mas[osn][st-1]*osn;
        }
    }

}

int fact(int num) {
    if (num>17) return -1; // при вводе числа больше 17 происходит переполнение
    return (num<2) ? 1 : num*fact(num-1);
}

int isInInt(const char*s){ // для проверки можно ли с этим числом работать в дальнейшем
    if (strlen(s)>10) return 0;
    if(strlen(s)==10){
        if (s[0]!='1' && s[0]!='2') return 0;
        if (s[0]=='2' && s[1]!='1' && s[1]!='0') return 0;
        if ( s[0]=='2' && s[1]=='1'){
            for (int i=2; i<=9; i++){
                if (s[i]!='0') return 0;
                }
        }
    return 1;
    }
}


int isInt(const char*s){ //проверка нет ли посторонних символов в числе  
    int i = 0;
    if (s[0]=='-')  i=1;
    while(s[i]){
        if (!isdigit(s[i])) return 0;
        i++;
    }
    return 1;
}