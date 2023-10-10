#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
enum Flag_str{
    OK,
    ERROR_MEMORY,
    ZERO_STR
};

void freeForFlagN(char* mas, char* mas1, char* mas2){
    free(mas);
    free(mas1);
    free(mas2);
}

void add_mas(char* startMas, char* endMas, int count, int indexStart){
    for (int i = 0; i <= count; i++){
        startMas[indexStart] = endMas[i];
        indexStart++;
    }
}

int FlagL(char* str){ //длина
    int i = 0;
    while(str[i]){
        i++;
    }
    return i;
}

enum Flag_str FlagR(char* str, char** res){ //перевертыш строки
    int len = FlagL(str);
    if (len == 0){
        return ZERO_STR;
    }
    *res = (char*)malloc(sizeof(char) * len); 
    if (!res){
        return ERROR_MEMORY;
    }
    len--;
    int m = 0;

    while (len >= 0){
        (*res)[m] = str[len];
        len--;
        m++;
    }
    return OK;
}

enum Flag_str FlagU(char* str, char** res){ //буквы на нечетных позициях в верхний регистр
    int len = FlagL(str);
    if (len == 0){
        return ZERO_STR;
    }
    *res = (char*)malloc(sizeof(char) * len);
    if(!res){
        return ERROR_MEMORY;
    }
    for (int i = 0; i < len; i++){
        if (isalpha(str[i]) && i % 2 != 0)
        {
            (*res)[i] = toupper(str[i]);
        }
        else {(*res)[i] = str[i];}
    }
    return OK;
}

enum Flag_str FlagN(char* str, char** res){ //сначала цифры потом буквы потом остальное

    int len = FlagL(str) - 1;
    if (len == -1){
        return ZERO_STR;
    }

    *res = (char*)malloc(sizeof(char) * len);
    char* temp;
    if (!res){
        return ERROR_MEMORY;
    }

    char* digits = (char*)malloc(sizeof(char) * 2); digits[1] = '\0';
    char* alphas = (char*)malloc(sizeof(char) * 2); alphas[1] = '\0';
    char* others = (char*)malloc(sizeof(char) * 2); others[1] = '\0';
    if (!digits || !alphas || !others){
        freeForFlagN(digits, alphas, others);
        return ERROR_MEMORY;
    }

    int d = 0; int a = 0; int o = 0;

    for (int i = 0; i <= len; i++){
        if (isdigit(str[i])){

            if (digits[d] == '\0'){
                temp = realloc(digits, sizeof(char) * (d + 1));
                if (!temp){
                    freeForFlagN(digits, alphas, others);
                    return ERROR_MEMORY;
                }

                digits = temp;
                digits[d+1] = '\0'; 
            }
            digits[d] = str[i];
            d++;
        }
        else if (isalpha(str[i])){

            if (alphas[a] == '\0'){
                temp = realloc(alphas, sizeof(char) * (a + 1));
                if (!temp){
                    freeForFlagN(digits, alphas, others);
                    return ERROR_MEMORY;
                }

                alphas = temp;
                alphas[a+1] = '\0'; 
            }
            alphas[a] = str[i];
            a++;
        }
        else {

            if (others[o] == '\0'){
                temp = realloc(others, sizeof(char) * (o + 1));
                if (!temp){
                    freeForFlagN(digits, alphas, others);
                    return ERROR_MEMORY;
                }

                others = temp;
                others[o+1] = '\0'; 
            }
            others[o] = str[i];
            o++;
        }
    }
    digits[d] = '\0';
    alphas[a] = '\0';
    others[o] = '\0';

    add_mas(*res, digits, d, 0);
    add_mas(*res, alphas, a, d);
    add_mas(*res, others, o, d+a);

    freeForFlagN(digits, alphas, others);
    return OK;
}


int main(int argc, char*argv[])
{
    if (argc < 3 || argc > 10 || argv[1][0] != '-' || (argv[1][1] != 'l' && argv[1][1] != 'r' && argv[1][1] != 'u' && argv[1][1] != 'n' && argv[1][1] != 'c')){
        printf("Неверный формат ввода.\n");
        printf("Возможные флаги: -l, -r, -u, -n, -c.\n");
        return 1;
    }

    if (argv[1][1] == 'l'){
            printf("Длина равна %d\n", FlagL(argv[2]));
            return 1;
    }
        

    if (argv[1][1] == 'r'){
            char* res = NULL;
            enum Flag_str ans = FlagR(argv[2], &res);
            if (ans != OK) {
                if (ans == ERROR_MEMORY){
                    printf("Не удалось выделить память.\n");
                    return 1;
                }
                else {
                    printf("Вы не ввели строку\n");
                    return 1;
                }
            }
            int i = 0;
            while (res[i]){
                printf("%c", res[i]);
                i++;
            }
            printf("\n");
            free(res);
            return 1;
    }

    if (argv[1][1] == 'u'){
        char* res;
        enum Flag_str ans = FlagU(argv[2], &res);
        if (ans != OK) {
                if (ans == ERROR_MEMORY){
                    printf("Не удалось выделить память.\n");
                    return 1;
                }
                else {
                    printf("Вы не ввели строку\n");
                    return 1;
                }
            }
            int i = 0;
            while (res[i]){
                printf("%c", res[i]);
                i++;
            }
            printf("\n");
            free(res);
            return 1;
    }


    if (argv[1][1] == 'n'){
            char *res;
            enum Flag_str ans = FlagN(argv[2], &res);
            if (ans != OK) {
                if (ans == ERROR_MEMORY){
                    printf("Не удалось выделить память.\n");
                    return 1;
                }
                else {
                    printf("Вы не ввели строку\n");
                    return 1;
                }
            }
            int i = 0;
            while (res[i]){
                printf("%c", res[i]);
                i++;
            }
            printf("\n");
            free(res);
            return 1;
    }

    if (argv[1][1] == 'c'){

        int count = 0;
        for (int i = 3; i < argc; i++){
            count+=FlagL(argv[i]);
        }

        char* res = (char*)malloc(sizeof(char) * count);
        if (!res){
            printf("Не удалось выделить память.\n");
            return 1;
        }

        unsigned int seed = atoi(argv[2]); //проверить все

        int k = 0;
        int max = argc - 1;
        srand(seed);
        int mas[argc]; 
        for (int i = 2; i <= max; i++){
            int ind = 2 + rand() % (max - 1);
            while (mas[ind] == -1){
                  ind = 2 + rand() % (max - 2 + 1);
            }
            int len = FlagL(argv[ind]);
            add_mas(res, argv[ind], len, k);
            k += len;
            mas[ind] = -1;
        }

        while(*res){
            printf("%c", *res);
            res++;
        }
        printf("\n");
        free(res);
        return 1;
    }

    return 0;
}