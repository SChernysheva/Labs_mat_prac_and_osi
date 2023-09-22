#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "functions.h"


int main(int argc, char*argv[]){
  
   if(((argc<2) || (strlen(argv[2])!=2)) || ((argv[2][1]!='h') && (argv[2][1]!='f') && (argv[2][1]!='p') && (argv[2][1]!='s') && (argv[2][1]!='e') && (argv[2][1]!='a') && (argv[2][0]!='/') ||\
    (argv[2][0]!='-') || (isInInt(argv[1])==0) || (isInt(argv[1])!=1 ))){
    printf("Неправильный формат ввода. Первый аргумент-целое число, второй флаг.\n");
    printf("Максимально возможное число для ввода - 2100000000\n");
    printf("Число 0 - невозможно, так как не подходит ни под один флаг.\n");
    printf("Возможные флаги: -h, -p, -s, -e, -a, -f\n");
    printf("Символ перед флагом: - или /\n");
    return 0;
   } //проверка на корректность ввода

   int num = atoi(argv[1]); //перевод числа в инт для дальнейшего использования в функциях
   int absNum = abs(num); //модуль числа, используется с некоторыми флагами
   


   if(argv[2][1]=='h'){  // ФЛАГ H

   if (absNum>100){  
    printf("Нет чисел в пределах 100, кратных указанному\n");
    return 0;
   }
   int sizeOfMas = 100/absNum;
   int result[sizeOfMas]; //массив для записи ответов

   flagH(absNum, result);

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
    char *res;
    int count=strlen(argv[1]); 
    flagS(num, &res, &count);
    if(count==-1) {
        printf("Недостаточно памяти.\n");
        free(res);
        return 0;
    }
    if(res[count]==0) count-=1;
    for (int i=count; i>=0; i--){
        printf("%d ", res[i]);
    }
    free(res);
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
    int mas[osn+1][degree+1]; // двумерный массив для рез-тов
    
    flagE(degree+1, mas); 

    for (int osn=0; osn<=10; osn++){
            printf("\n");
            for (int j=0; j<=degree; j++){
                printf("%d^%d = %d ", osn, j, mas[osn][j]);
            }
    }

    printf("\n");
    return 0;
   }



   if(argv[2][1]=='a'){
    (flagA(num)==-1) ? printf("Вы ввели отрицательное число, сумма натуральных чисел до указанного равна 0.\n") : printf("Сумма всех натуральных чисел до %d = %lld\n", num, flagA(num));
    return 0;
   }


   if(argv[2][1]=='f'){
    if (num<0)  printf("Факториал отрицательного числа не существует, расчет будет произведен по модулю.\n");
    (fact(num)==-1) ? printf("Вы ввели слишком большое число, результат не помещается в int.\n") : printf("Факториал %d = %d\n", absNum, fact(absNum));
    return 0;
   }



    return 0;
}
