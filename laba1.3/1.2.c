#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>


int checkArgs(char flag, int count, char*argv[]){ //проверяем аргументы и соответствие их кол-ва флагу
    if ((flag=='q' && count!=6) || (flag=='m'  && count!=4) || (flag=='t' && count!=6)) return 0;
    for (int i=2; i<count; i++)
    {
        if (!isNubmer(argv[i])) return 0;
    }
    return 1;
}

int isEqual(float num1, float num2, float eps) //проверка равенства вещ чисел
{
     return (fabs(num1-num2)<eps) ? 1 : 0;
}

float getDiscr(float num1, float num2, float num3) //расчет дискриминанта
{
    return num2*num2-4*num1*num3;
}

void getAnswersT(float mas[][3], int count, float masAnswers[][3], float eps) //функция для флага -q, mas- массив массивов с count числом перестановок
{
    for (int i=0; i<count; i++)
    {
        if (isEqual(mas[i][0], 0, eps)) //если первый элем (x^2) =0 
        {
            if (!isEqual(mas[i][1], 0, eps)) //а второй(x) не равен
            {
                masAnswers[i][0]=1;
                masAnswers[i][1]=-mas[i][2]/mas[i][1];
            }
            else //если второй тоже равен
            {
                if (isEqual(mas[i][2], 0, eps)) // если третий равен то ответ все числа (0+0+0=0)
                {
                    masAnswers[i][0]=3;
                } 
                else masAnswers[i][0]=0; //если своб коэф не равен нулю - ответов нет (0+0+5=!0)
            }
            continue;
        }

        if (isEqual(mas[i][1], 0, eps)) 
        {
            if ((0+mas[i][2]>eps && 0+mas[i][0]>eps)) //если у чисел одинаковые знаки то при переносе знаки будут отличаться (напр 3x^2+2 => 3x^2=-2 => нет корней)
            {
                masAnswers[i][0]=0;
            }
            else 
            {
                masAnswers[i][0]=2;
                masAnswers[i][1]=sqrt(-mas[i][2]/mas[i][0]);
                masAnswers[i][2]=-(sqrt(-mas[i][2]/mas[i][0]));
            }
            continue;
        }


        float d=getDiscr(mas[i][0], mas[i][1], mas[i][2]);
        if ((0+d)<-eps) 
        {
            masAnswers[i][0]=0; 
            continue;
        }
        masAnswers[i][0]=2;
        masAnswers[i][1] = (-mas[i][1]+sqrt(d))/(2*mas[i][0]);
        masAnswers[i][2] = (-mas[i][1]-sqrt(d))/(2*mas[i][0]);
    }
}

void getPermutations(float mas[][3], float num1, float num2, float num3, int count, float eps)
{
    if (count==1) 
    {
        mas[0][0]=num1;
        mas[0][1]=num2;
        mas[0][2]=num3;
        return;
    }
    if (count==3)
    {
        if (isEqual(num1, num2, eps)) //делаем расстановку когда первые два числа в массиве - равны, третье будем перемещать по индексам
        {
            mas[0][0]=num1;
            mas[0][1]=num2;
            mas[0][2]=num3;
        }
        if (isEqual(num2,num3, eps))
        {
            mas[0][0]=num2;
            mas[0][1]=num3;
            mas[0][2]=num1;
        }
        if (isEqual(num1, num3, eps))
        {
            mas[0][0]=num1;
            mas[0][1]=num3;
            mas[0][2]=num2;
        }
        for (int i=1; i<3; i++)
        {
            if (i==1) 
            {
                mas[i][0]=mas[i-1][0];
                mas[i][1]=mas[i-1][2];
                mas[i][2]=mas[i][0];
            }
            if (i==2)
            {
                mas[i][0]=mas[i-1][1];
                mas[i][1]=mas[i-1][0];
                mas[i][2]=mas[i-1][2];
            }
        }
    }
    if (count==6)
    {
        mas[0][0]=num1;
        mas[0][1]=num2;
        mas[0][2]=num3;
        for (int i=1; i<6; i++)
        {
            if(i==4) 
            {
                mas[i][0]=mas[i-1][1];
                mas[i][1]=mas[i-1][2];
                mas[i][2]=mas[i-1][0];
            }
            else if(i%2!=0)
            {
                mas[i][0]=mas[i-1][0];
                mas[i][1]=mas[i-1][2];
                mas[i][2]=mas[i-1][1];
            }
            else 
            {
                mas[i][0]=mas[i-1][2];
                mas[i][1]=mas[i-1][0];
                mas[i][2]=mas[i-1][1];
            }
        }
    }

}

int isNubmer(const char*s){ //проверка нет ли посторонних символов в числе  
    int i = 0;
    if (s[0]=='-')  i=1;
    while(s[i]){
        if (!isdigit(s[i]) && s[i]!='.' && s[i]!='e' && s[i]!='-') return 0;
        i++;
    }
    return 1;
}

int flagM(int num1, int num2)
{
    if (num2==0) return -1;
    return (num1%num2==0) ? 1 : 0;
}

int flagT(float a, float b, float c, float eps)
{
    float res1=b*b+c*c;
    float res2=a*a+c*c;
    float res3=b*b+a*a;
    if ((fabs(a*a-res1)<eps) || (fabs(b*b-res2)<eps) || (fabs(c*c-res3)<eps)) return 1;
    return 0;
}


int main(int argc, char*argv[])
{
    if(((argc<4 || argc>6) || (strlen(argv[1])!=2)) || ((argv[1][1]!='q') && (argv[1][1]!='m') && (argv[1][1]!='t') ) || ((argv[1][0]!='-') && (argv[1][0]!='/')) )
    {
            printf("Неправильный формат ввода. Первый аргумент-флаг, затем аргументы для него.\n");
            printf("Возможные флаги: -q, -m, -t \n");
            printf("Символ перед флагом: - или /\n");
            return 0;
    } //проверка на корректность ввода

    if(checkArgs(argv[1][1], argc, argv)==0) 
    {
        printf("Несоответсвие кол-ва аргументов флагу или неверно введены числа.\n");
        return 0;
    }

    if (argv[1][1]=='m'){
        int num1=atoi(argv[2]);
        int num2=atoi(argv[3]);
        int res=flagM(num1, num2);
        if (res!=-1)
        {
            (res==1) ? printf("Кратно\n") : printf("Некратно\n");
            return 0;
        }
        printf("Одно из чисел - ноль\n");
        return 0;
    }

    if (argv[1][1]=='t')
    {
        float eps=atof(argv[2]);
        float num1=atof(argv[3]);
        float num2=atof(argv[4]);
        float num3=atof(argv[5]);
        flagT(num1, num2, num3, eps)==1 ? printf("Могут\n") : printf("Не могут\n");
        return 0;
    }

    if (argv[1][1] == 'q') 
    {
        float eps=atof(argv[2]);
        float num1=atof(argv[3]);
        float num2=atof(argv[4]);
        float num3=atof(argv[5]);
        int countPerest = 0;
        if (isEqual(num1, num2, eps) && isEqual(num2, num3, eps)) countPerest=1;
        else if (isEqual(num1, num2, eps) || isEqual(num1, num3, eps) || isEqual(num2, num3, eps)) countPerest=3;
        else countPerest=6;
        float mas[countPerest][3]; //массив всевозможных перестановок
        getPermutations(mas, num1, num2, num3, countPerest, eps);
        float masAnswers[countPerest][3]; //массив ответов, 0 индекс - кол-во ответов
        getAnswersT(mas, countPerest, masAnswers, eps);

        for(int i=0; i<countPerest; i++)
        {
            printf("Для перестановки ");
            printf("%f, %f, %f ", mas[i][0], mas[i][1], mas[i][2]);
            printf( "ответ = ");
            if (masAnswers[i][0]==0) printf("Нет корней\n");
            else if (masAnswers[i][0]==2) printf("%f, %f\n", masAnswers[i][1], masAnswers[i][2]);
            else if (masAnswers[i][0]==3) printf("любые числа\n");
            else printf("%f\n", masAnswers[i][1]);
        }
    }

}