#include <stdio.h>
#include <string.h>
#include <math.h>
#include "functions.h"


/*void flagH(int num, int result[]) {
    for (int c=num; c<=100; c+=num){ 
        result[c] = 1; //числа, кратные заданному будем вычислять добавляя на каждом шагу ему равное и помечая номер массива единицей
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

int flagA(int num){
    if(num<0) {
        return -1;
    }
    int res=0;
    for (int i=1; i<=num; i++){
        res+=i;
    }
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
    return (num<2) ? 1 : num*fact(num-1);
}


int Atoi(const char*s){ //Ручная реализация функции атои
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
} */


int main(int argc, char*argv[]){
  
   if(((argc<2) || (strlen(argv[2])!=2)) || ((argv[2][1]!='h') && (argv[2][1]!='f') && (argv[2][1]!='p') && (argv[2][1]!='s') && (argv[2][1]!='e') && (argv[2][1]!='a') && (argv[2][0]!='/') ||\
    (argv[2][0]!='-') || (Atoi(argv[1])==0) || (isInt(argv[1])!=1 ))){
    printf("Неправильный формат ввода. Первый аргумент-целое число, второй флаг.\n");
    printf("Число 0 - невозможно, так как не подходит ни под один флаг.\n");
    printf("Возможные флаги: -h, -p, -s, -e, -a, -f\n");
    printf("Символ перед флагом: - или /\n");
    return 0;
   } //проверка на корректность ввода

   int num = Atoi(argv[1]); //перевод числа в инт для дальнейшего использования в функциях
   int absNum = abs(num); //модуль числа, используется с некоторыми флагами
   


   if(argv[2][1]=='h'){  // ФЛАГ H

   if (absNum>100){  
    printf("Нет чисел в пределах 100, кратных указанному\n");
    return 0;
   }
   int sizeOfMas = 100/absNum;
   int result[sizeOfMas]; //массив для записи ответов

   flagH(absNum, result, sizeOfMas);

   printf("Числа, кратные %d: ", num);

   for (int i=0; i<sizeOfMas; i++){
        printf("%d ", result[i]);
    }
    printf("\n");
    
    return 0;
   }



   if(argv[2][1]=='p'){ //Флаг P
    (flagP(absNum)==1) ? printf("Число %d - составное\n", num) : printf("Число %d - простое\n", num);
    return 0;
   }


   if(argv[2][1]=='s'){  //Флаг S
    int count=strlen(argv[1]); 
    char res[count]; //массив для записи ответов
    flagS(argv[1], count, res);
    for (int i=0; i<=count; i++){
        printf("%c ", res[i]);
    }
    printf("\n");
    return 0;
   }


   if(argv[2][1]=='e'){ //Флаг E

    if(absNum>10) {
        printf("Для этого флага число не может быть больше 10\n"); 
        return 0;
    }

    int osn=10; //основание по условию
    int degree=absNum; //степень берем в любом случае положительную, поэтому используем модуль
    float mas[osn+1][degree+1]; // двумерный массив для рез-тов
    
    flagE(degree+1, mas); 

    for (int osn=0; osn<=10; osn++){
            printf("\n");
            for (int j=0; j<=degree; j++){
                printf("%d^%d = %d ", osn, j, (int)mas[osn][j]);
            }
    }

    printf("\n");
    return 0;
   }



   if(argv[2][1]=='a'){
    (flagA(num)==-1) ? printf("Вы ввели отрицательное число, сумма натуральных чисел до указанного равна 0.\n") : printf("Сумма всех натуральных чисел до %d = %d\n", num, flagA(num));
    return 0;
   }


   if(argv[2][1]=='f'){
    if (fact(num)==-1)  printf("Факториал отрицательного числа не существует, расчет будет произведен по модулю.\n");
    printf("Факториал %d = %d\n", absNum, fact(absNum));
    return 0;
   }



    return 0;
}
