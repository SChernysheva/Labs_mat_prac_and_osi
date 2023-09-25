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
        printf("%f\n", d);
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

void swap(float mas[], int i1, int i2){
    float temp=mas[i1]; 
    mas[i1]=mas[i2]; 
    mas[i2]=temp;
    return;
}

void getPermutations(float masA[][3], float mas[], int*i, int k){ //Алгоритм Хипа, масА- массив для ответов, мас-массив с элементами, i - указатель для записи ответов, k - элемент из алгоса
    if (k==1) 
    {
        masA[*i][0]=mas[0];
        masA[*i][1]=mas[1];
        masA[*i][2]=mas[2];
        (*i)++;
        return;
    }
    for (int m=0; m<k; m++){
        getPermutations(masA, mas, i, k-1);
        if (k%2==0){
            swap(mas, m, k-1);
        }
        else swap(mas, 0, k-1);
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
    float res1=a*a;
    float res2=b*b;
    float res3=c*c;
    if ((fabs(res1-(res2+res3))<eps) || (fabs(res2-(res1+res3)<eps) || (fabs(res3-(res1+res2))<eps))) return 1;
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
        int i=0;
        int*iptr=&i;
        float mas[countPerest][3]; //массив всевозможных перестановок
        float m[3]={num1, num2, num3};
        getPermutations(mas, m, iptr, 3);
        //getPermutations(mas, num1, num2, num3, countPerest, eps);
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