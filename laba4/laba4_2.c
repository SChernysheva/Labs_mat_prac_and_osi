#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};
typedef struct 
{
    int* array;
    int capasity;
    int size;
    char name;
} mas;
mas* create_mas(int capasity, char name)
{
    mas* res = (mas*)malloc(sizeof(mas));
    if (!res)
    {
        return NULL;
    }
    res->array = (int*)malloc(sizeof(int) * capasity);
    res->capasity = capasity;
    res->name = name;
    res->size = 0;
}
int checkInt(char* str)
{
    while(*str)
    {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}
enum checker change_capasity(mas* data, int user_capasity)
{
    int* temp = realloc(data->array, user_capasity);
    if (!temp)
    {
        return ERROR_MEMORY;
    }
    data->array = temp;
    data->capasity = user_capasity;
    return OK;
}

enum checker fLoad(char* str, mas** all_mass)
{ // A, in.txt
    if (!isalpha(str[0])) return INVALID;
    char name_mas = toupper(str[0]);
    mas* arr = all_mass[(int)name_mas - 65];
    FILE* file = fopen(&(str[3]), "r");
    if (!file)
    {
        return INVALID;
    }
    char num[50];
    int index_num = 0;
    char c = fgetc(file);
    while (c != EOF)
    {
        if (isdigit(c))
        {
            while(isdigit(c))
            {
                num[index_num] = c;
                index_num++;
                c = fgetc(file);
            }
            num[index_num] = '\0';
            arr->array[arr->size] = atoi(num);
            (arr->size)++;
            if (arr->size >= arr->capasity)
            {
                enum checker ans = change_capasity(arr, arr->capasity * 2);
                if (ans != OK)
                {
                    return ans;
                }
            }
        }
        index_num = 0;
        c = fgetc(file);
    }
    fclose(file);
}

enum checker fRand(char* str, mas** all_mass)
{//A, count, lb, rb
    if (!isalpha(str[0])) return INVALID;
    char name_mas = toupper(str[0]);
    mas* arr = all_mass[(int)name_mas - 65];
    char* str_after_mas = &(str[3]);
    int i = 0;
    char first_num[10], second_num[10], count[5];
    while (isdigit(str_after_mas[i]))
    {
        count[i] = str_after_mas[i];
        i++;
    }
    count[i] = '\0';
    if (!checkInt(count)) return INVALID;
    int num_count = atoi(count);
    if (num_count + arr->size >= arr->capasity)
    {
        if (change_capasity(arr, arr->capasity * 2 * num_count) != OK)
        {
            return ERROR_MEMORY;
        }
    }
    i += 2;
    int index_ = 0;
    while (isdigit(str_after_mas[i]))
    {
        first_num[index_] = str_after_mas[i];
        index_++;
        i++;
    }
    first_num[index_] = '\0';
    if (!checkInt(first_num)) return INVALID;
    int num_first = atoi(first_num);
    i += 2;
    index_ = 0;
    while (isdigit(str_after_mas[i]))
    {
        second_num[index_] = str_after_mas[i];
        index_++;
        i++;
    }
    second_num[index_] = '\0';
    if (!checkInt(second_num)) return INVALID;
    int num_second = atoi(second_num);
    if (num_first > num_second) return INVALID;
    srand(time(0));
    printf("%d %d %d\n", num_count, num_first, num_second);
    for (int k = 0; k < num_count; k++)
    {
        arr->array[arr->size] = rand() % (num_second + 1) + num_first;
        (arr->size)++;
    }
    return OK;

}

enum checker fSave(char* str, mas** all_mass)
{
    if (!isalpha(str[0])) return INVALID;
    char name_mas = toupper(str[0]);
    mas* arr = all_mass[(int)name_mas - 65];
    FILE* file = fopen(&(str[3]), "w");
    if (!file)
    {
        return INVALID;
    }
    for (int i = 0; i < arr->size; i++)
    {
        fprintf(file, "%d", arr->array[i]);
    }
    return OK;
}

enum checker fPrintf(char* str, mas** all_mass)
{//a, 3
    if (!isalpha(str[0])) return INVALID;
    char name_mas = toupper(str[0]);
    if (str[1] != ',') return INVALID;
    mas* arr = all_mass[(int)name_mas - 65];
    char first_num[4];
    char second_num[4];
    char* str_after_print = &(str[3]);
    int i = 0;
    if (strcmp(str_after_print, "all") == 0)
    {
        for (int i = 0; i < arr->size; i++)
        {
            printf("%d ", arr->array[i]);
        }
        return OK;
    }
    while (isdigit(str_after_print[i]))
    {
        first_num[i] = str_after_print[i];
        i++;
    }
    first_num[i] = '\0';
    if (str_after_print[i] != ',')
    {
        if (!checkInt(first_num))
        {
            return INVALID;
        }
        int index = atoi(first_num);
        if (index < 0 || index > arr->size) return INVALID;
        printf("%d\n", arr->array[index]);
        return OK;
    }
    else if (str_after_print[i] == ',')
    {
        i += 2;
        int index_second_num = 0;
        while (isdigit(str_after_print[i]))
        {
            second_num[index_second_num] = str_after_print[i];
            i++;
            index_second_num++;
        }
        second_num[index_second_num] = '\0';
        if (!checkInt(first_num) || !checkInt(second_num))
        {
            return INVALID;
        }
        int index_1 = atoi(first_num);
        int index_2 = atoi(second_num);
        if (index_2 < index_1 || index_2 > arr->size || index_1 < 0) return INVALID;
        for (int m = index_1; m <= index_2; m++)
        {
            printf("%d ", arr->array[m]);
        }
        return OK;
    }
    
}

int main(char* argv[], int argc, ...)
{
    mas* all_mass[26];
    for (int i = 65; i <= 91; i++)
    {
        all_mass[i - 65] = create_mas(rand() % 20, (char)i);
        if (all_mass[i - 65] == NULL)
        {
            printf("Ошибка памяти\n");
            //free(all_mass);
            for (int m = 0; m < i - 65; m++)
            {
                free(all_mass[m]);
            }
        }
    }
    char user_str[50];
    int k = 0;
    printf("Введите инструкцию, print или exit для выхода\n");
    char c;
    while ((c = getchar()) != '\n')
    {
        user_str[k] = c;
        k++;
    }
    user_str[k] = '\0';
    while (strcmp(user_str, "exit") != 0)
    {
        char instruction[7];
        int i = 0;
        while (user_str[i] != ' ')
        {
            instruction[i] = user_str[i];
            i++;
        }
        int index_dop_info = i + 1;
        instruction[i] = '\0';
        if (strcmp(instruction, "Load") == 0) //10
        {
            if (fLoad(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Ошибка\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Save") == 0)
        {//Save A, out.txt; - выгрузить элементы массива A в файл out.txt;
            if (fSave(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Ошибка\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Rand") == 0)
        {
            if (fRand(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Ошибка в выделении памяти оибо инвалидные входные данные\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Concat") == 0)
        {
            
        }
        else if (strcmp(instruction, "Print") == 0)
        {
            if (fPrintf(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Инвалидные входные данные\n");
            }
        }
        int k = 0;
        printf("Введите инструкцию, print или exit для выхода\n");
        char c;
        while ((c = getchar()) != '\n')
        {
            user_str[k] = c;
            k++;
        }
        user_str[k] = '\0';
    }
}