#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include "structures.h"
enum answer do_app(int num, int max_pr, int count_otd, char* arr[])
{
    //<время поступления заявки> <приоритет> <идентификатор отделения> “<текст заявки>”
    char str[9];
    snprintf(str, sizeof(str), "app%d.txt", num);
    FILE* file = fopen(str, "w");
    if (!file) return INVALID;
    time_t current_time;
    struct tm *time_info;
    char time_string[20]; // Длина 20 достаточна для формата ISO8601
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%S", time_info);
    fprintf(file, "%s ", time_string);
    int prior = rand() % max_pr + 1;
    fprintf(file, "%d ", prior);

    int index = rand() % count_otd;

    fprintf(file, "%s ", arr[index]);
    int size_app = rand() % 100 + 1;
    char* str1 = (char*)malloc(sizeof(char) * size_app);
    if (!str1) return ERROR_MEMORY;
    for(int i = 0; i < size_app; i++) {
        str1[i] = 'a' + rand() % 26; // генерируем случайную букву от 'a' до 'z'
    }
    str1[size_app] = '\0'; // добавляем символ конца строки
    fprintf(file, "%s", str1);
    free(str1);
    fclose(file);
}
enum answer do_apps(int count, int max_pr, int count_otd, char* arr[])
{
    if (count >= 10) return INVALID;
    
    for (int i = 1; i < count + 1; i++)
    {
        if (do_app(i, max_pr, count_otd, arr) != OK) return ERROR_MEMORY;
    }
}
char** do_context_dialog_user(int* count)
{
    FILE* file = fopen("test.txt", "w");
    int digit = -1;
    char c;
    while (digit < 1 || digit > 6)
    {
    printf("Выберите тип кучи:\n");
    printf(" 1) BinaryHeap\n 2) BinomialHeap\n 3) FibonacciHeap\n 4) SkewHeap\n 5) LeftistHeap\n 6) Treap\n");
    scanf("%d", &digit);
    }
    switch (digit)
    {
        case 1:
        fprintf(file, "BinaryHeap\n");
        break;
        case 2:
        fprintf(file, "BinomialHeap\n");
        break;
        case 3:
        fprintf(file, "FibonacciHeap\n");
        break;
        case 4:
        fprintf(file, "SkewHeap\n");
        break;
        case 5:
        fprintf(file, "LeftistHeap\n");
        break;
        case 6:
        fprintf(file, "Treap\n");
        break;
    }
    digit = -1;
    while (digit < 1 || digit > 4)
    {
    printf("Выберите тип хранилища:\n");
    printf(" 1) HashSet\n 2) DynamicArray\n 3) BinarySearchTree\n 4) Trie\n");
    scanf("%d", &digit);
    }
    switch (digit)
    {
        case 1:
        fprintf(file, "HashSet\n");
        break;
        case 2:
        fprintf(file, "DynamicArray\n");
        break;
        case 3:
        fprintf(file, "BinarySearchTree\n");
        break;
        case 4:
        fprintf(file, "Trie\n");
        break;
    }
    time_t current_time;
    struct tm *time_info;
    char time_string[20]; // Длина 20 достаточна для формата ISO8601
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%S", time_info);

    fprintf(file, "%s\n", time_string);
    printf("Введите время конца моделирования в формате %s\n", time_string);
    scanf("%s", time_string);
    fprintf(file, "%s\n", time_string);

    digit = -1;
    while (digit < 0)
    {
    printf("Введите минимальное время обработки заявки\n");
    scanf("%d", &digit);
    }
    fprintf(file, "%d\n", digit);

    digit = -1;
    while (digit < 0)
    {
    printf("Введите максиальное время обработки заявки\n");
    scanf("%d", &digit);
    }
    fprintf(file, "%d\n", digit);

    int count_otd = -1;
    while (count_otd < 1 || count_otd > 100)
    {
    printf("Введите кол-во отделений\n");
    scanf("%d", &count_otd);
    }
    fprintf(file, "%d\n", count_otd);
    *count = count_otd;
    for (int i = 0; i < count_otd; i++)
    {
    printf("Введите кол-во операторов для отделения %d от 10 до 50\n", i);
    scanf("%d", &digit);
    fprintf(file, "%d ", digit);
    }
    fprintf(file, "\n");

    char **arr = (char **)malloc(count_otd * sizeof(char *));
    if (!arr) return ERROR_MEMORY;
    for (int i = 0; i < count_otd; i++)
    {
        char ident_otd[4];
        for (int l = 0; l < 3; l++)
        {
            //srand(time(0));
            ident_otd[l] = 'a' + rand() % 26; // генерируем случайную букву от 'a' до 'z'
        }
        ident_otd[3] = '\0'; // добавляем символ конца строки
        arr[i] = (char*)malloc(4 * sizeof(char));
        strcpy(arr[i], ident_otd);
        fprintf(file, "%s ", arr[i]);
    }
    fprintf(file, "\n");
    double overload_coeff = -1;
    digit = -1;
    while (overload_coeff < 1)
    {
    printf("Введите коэф переполнения\n");
    scanf("%lf", &overload_coeff);
    }
    fprintf(file, "%f\n", overload_coeff);
    fclose(file);
    return arr;
}

int main(int argc, char *argv[])
{
    int max_pr = atoi(argv[1]);
    int count_apps = 5;
    int count_otd;
    char** arr = do_context_dialog_user(&count_otd);
    if (do_apps(count_apps, max_pr, count_otd, arr) != OK)
    {
        printf("Ошибка\n");
    }
    for (int i = 0; i < count_otd; i++)
    {
        free(arr[i]);
    }
    free(arr);
}