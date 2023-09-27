#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

int isInInt(char*s){ // для проверки переполнения 
    if (strlen(s)>10) return 0;
    if(strlen(s)==10){
        if (s[0]!='1' && s[0]!='2') return 0;
        if ( s[0]=='2'){
            for (int i=1; i<=9; i++){
                if (s[i]!='0') return 0;
                }
        }
    return 1;
    }
}
int isInt(const char*s, int flag){ //проверка нет ли посторонних символов в числе(flag = 1 если надо проверить инт и 0 если float)
    int i = 0;
    if (flag==1){
        if (s[0]=='-')  i=1;
        while(s[i]){
            if (!isdigit(s[i])) return 0;
            i++;
        }
    }
    else{
        while(s[i]){
            if (!isdigit(s[i]) && s[i]!='-' && s[i]!='e') return 0;
            i++;
        }
    }
    return 1;
}

long long int firstTask(int x, float eps){
    int n=0;
    long long int xpow; //предыдущее значение x^n
    long long int nfact; //предыдущее значение n!
    long long int s1=0; //суммы для проверки s1-предыдущая и s2 - текущая
    long long int s2=eps+10;;

    while(abs(s2)-abs(s1)>eps){
        if(n==0){
            s1=1;
            n++;
        }
        else if (n==1) {
            s2 = x/1;
            nfact=1;
            xpow=x;
            n++;
        }
        else {
            s1=s2;
            xpow=xpow*x;
            nfact=nfact*n;
            s2=xpow/nfact;
            n++;
        }
    }
    if (abs(s2)-abs(s1)<=eps) {
        return s2;
    }
}


long long int secondTask(int x, float eps){
    int x2=x*x;
    int n=0;
    long long int xpow;
    long long int nfact;
    long long int s1=0;
    long long int s2=eps+10;

    while(abs(s2)-abs(s1)>eps){
        if(n==0){
            s1=1;
            n++;
        }
        else if (n==1) {
            s2 = -x2/2;
            nfact=1;
            xpow=x2;
            n++;
        }
        else {
            s1=s2;
            xpow=xpow*x2;
            nfact=nfact*(n-1)*n;
            s2=xpow/nfact;
            if (n%2!=0) s2=-s2;
            n++;
        }
    }
    if (abs(s2)-abs(s1)<=eps) {
        return s2;
    }
}

long long int thirdTask(int x, float eps){
    int n=0;
    int x2=x*x; //=x^2
    int ninepow; //=9^n
    long long int xpow;//=(x^2)^n
    long long int nfact;//=(3n)!
    long long int n3fact;//=(n!)^3
    long long int s1=0;
    long long int s2=eps+10;

    while(abs(s2)-abs(s1)>eps){
        if(n==0){
            s1=1;
            n++;
        }
        else if (n==1) {
            s2 = (9*x2)/6;
            ninepow=9;
            n3fact=6;
            nfact=1;
            xpow=x2;
            n++;
        }
        else {
            s1=s2;
            xpow=xpow*x2;
            n3fact=n3fact*pow(n,3);
            if(n==2) nfact=nfact*n*(n-1);
            else nfact=nfact*n*(n-1)*(n-2);
            s2=(ninepow*n3fact*xpow)/nfact;
            n++;
        }
    }
    if (abs(s2)-abs(s1)<=eps) {
        return s2;
    }
}

long long int fourthTask(int x, float eps){
    int x2=x*x;
    int n=1;
    long long int xpow;
    long long int nechfact; //(2n-1)!!
    long long int chfact;// (2n)!!
    long long int s1=0;
    long long int s2=eps+10;
    while(abs(s2)-abs(s1)>eps){
        if (n==1) {
            s2 = -x*x;
            nechfact=1;
            xpow=x2;
            n++;
        }
        else if(n==2){
            s1=s2;
            chfact=2;
            xpow=xpow*x2;
            s2=xpow/chfact;
            n++;
        }
        else {
            s1=s2;
            xpow=xpow*x2;
            if (n%2==0){
                chfact=chfact*n;
                s2=(xpow*nechfact)/chfact;
            }
            else {
                nechfact=nechfact*n;
                s2=(-xpow*nechfact)/chfact;
            }
            n++;
        }
    }
    if (abs(s2)-abs(s1)<=eps) {
        return s2;
    }
}
int main(int argc, char*argv[])
{
    if( (argc!=3))
    {
            printf("Неправильный формат ввода. Введите эпсилон(вещественный тип), а затем х.\n");
            return 0;
    } //проверка на корректность ввода


    float eps=atof(argv[1]);

    if (isInInt(argv[2])==0 || isInt(argv[2],1)==0 || isInt(argv[1], 0)==0){
        printf("Переполнение или неправильный формат ввода. Введите число(!) менее 2000000000 и эпсилон.\n");
        return 0;
    }

    int x=atoi(argv[2]);
    //1)
    printf("Для первого варианта суммы ответ: %lld\n", firstTask(x, eps));

    //2)
    printf("Для второго варианта суммы ответ: %lld\n", secondTask(x, eps));

    //3)
    printf("Для третьего варианта суммы ответ: %lld\n", thirdTask(x, eps));

    //4
    printf("Для четвертого варианта суммы ответ: %lld\n", fourthTask(x, eps));

    return 0;
}