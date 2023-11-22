#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};
typedef struct
{
    int *array;
    int capasity;
    int size;
    char name;
} mas;
mas *create_mas(int capasity, char name)
{
    mas *res = malloc(sizeof(mas));
    if (!res)
    {
        return NULL;
    }
    res->array = (int *)malloc(sizeof(int) * capasity);
    if (!res->array)
        return NULL;
    res->capasity = capasity;
    res->name = name;
    res->size = 0;
    return res;
}
int checkInt(char *str)
{
    if (str == NULL)
        return 0;
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
enum checker change_capasity(mas *data, int user_capasity)
{
    int *temp = realloc(data->array, sizeof(int) * user_capasity);
    if (!temp)
    {
        return ERROR_MEMORY;
    }
    data->array = temp;
    data->capasity = user_capasity;
    return OK;
}

enum checker fLoad(char *str, mas **all_mass)
{ // A, in.txt
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != ',' || str[2] != ' ')
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr = all_mass[(int)name_mas - 65];
    FILE *file = fopen(&(str[3]), "r");
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
            while (isdigit(c))
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
    return OK;
}

enum checker fRand(char *str, mas **all_mass)
{ // A, count, lb, rb
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != ',' || str[2] != ' ')
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr = all_mass[(int)name_mas - 65];
    char *str_after_mas = &(str[3]);
    int i = 0;
    char first_num[10], second_num[10], count[5];
    while (isdigit(str_after_mas[i]))
    {
        count[i] = str_after_mas[i];
        i++;
    }
    count[i] = '\0';
    if (!checkInt(count))
        return INVALID;
    int num_count = atoi(count);
    if (num_count >= 100000)
        return ERROR_MEMORY;
    if (num_count + arr->size >= arr->capasity)
    {
        if (change_capasity(arr, arr->capasity + num_count) != OK)
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
    if (!checkInt(first_num))
        return INVALID;
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
    if (!checkInt(second_num))
        return INVALID;
    int num_second = atoi(second_num);
    if (num_first > num_second)
        return INVALID;
    srand(time(0));
    // printf("%d %d %d\n", num_count, num_first, num_second);
    for (int k = 0; k < num_count; k++)
    {
        arr->array[arr->size] = rand() % (num_second + 1) + num_first;
        (arr->size)++;
    }
    return OK;
}

enum checker fConcat(char *str, mas **all_mass)
{ // A, b;
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != ',' || str[2] != ' ')
        return INVALID;
    if (!isalpha(str[3]))
        return INVALID;
    if (strlen(str) != 4)
        return INVALID;
    char name_mas = toupper(str[0]);
    char name_mas2 = toupper(str[3]);
    mas *arr = all_mass[(int)name_mas - 65];
    mas *arr2 = all_mass[(int)name_mas2 - 65];
    if (arr->capasity < arr->size + arr2->size)
    {
        if (change_capasity(arr, arr->size + arr2->size + 1) != OK)
        {
            return ERROR_MEMORY;
        }
    }
    int index_arr2 = 0;
    for (int i = 0; i < arr2->size; i++)
    {
        arr->array[arr->size] = arr2->array[i];
        (arr->size)++;
    }
    return OK;
}
enum checker fRemove(char *str, mas **al_mass)
{ // a, 2, 7
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != ',' || str[2] != ' ')
        return INVALID;
    if (!isdigit(str[3]))
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr = al_mass[(int)name_mas - 65];
    char num1[3];
    char num2[3];
    int i = 3;
    for (i; isdigit(str[i]); i++)
    {
        num1[i - 3] = str[i];
    }
    num1[i - 3] = '\0';
    i += 2;
    int index_num_2 = 0;
    for (i; isdigit(str[i]); i++)
    {
        num2[index_num_2] = str[i];
        index_num_2++;
    }
    num2[index_num_2] = '\0';
    if (!checkInt(num1) || !checkInt(num2))
        return INVALID;
    int num_index = atoi(num1); // 2
    int num_count = atoi(num2); // 3;
    if (num_index > arr->size || num_index < 0 || num_count < 0 || num_index + num_count > arr->size)
        return ERROR_MEMORY;

    int index_start = num_index;
    for (int i = index_start; i < arr->size - num_count; i++)
    {
        arr->array[i] = arr->array[i + num_count];
    }
    arr->size = arr->size - num_count;
    return OK;
}
enum checker fSave(char *str, mas **all_mass)
{
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != ',' || str[2] != ' ')
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr = all_mass[(int)name_mas - 65];
    FILE *file = fopen(&(str[3]), "w");
    if (!file)
    {
        return INVALID;
    }
    for (int i = 0; i < arr->size; i++)
    {
        fprintf(file, "%d ", arr->array[i]);
    }
    fclose(file);
    return OK;
}
// A, 4, 10, b; - скопировать из массива А элементы с 4 по 10 (оба конца
// включительно) и сохранить их в b;
enum checker fCopy(char *str, mas **all_mass)
{
    if (!isalpha(str[0]) || str[1] != ',' || str[2] != ' ' || !isdigit(str[3]))
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr1 = all_mass[(int)name_mas - 65];
    char first_num[10];
    char second_num[10];
    char *str_after_name = &(str[3]);
    int i = 0;
    while (isdigit(str_after_name[i]))
    {
        first_num[i] = str_after_name[i];
        i++;
    }
    first_num[i] = '\0';
    i += 2;
    int index_sec_num = 0;
    while (isdigit(str_after_name[i]))
    {
        second_num[index_sec_num] = str_after_name[i];
        i++;
        index_sec_num++;
    }
    second_num[index_sec_num] = '\0';
    if (strlen(first_num) == 0 || strlen(second_num) == 0)
        return INVALID;
    i += 2;
    if (!isalpha(str_after_name[i]))
        return INVALID;
    char name_mas2 = toupper(str_after_name[i]);
    mas *arr2 = all_mass[(int)name_mas2 - 65];
    int index1 = atoi(first_num);
    int index_2 = atoi(second_num);
    if (index1 > index_2 || index_2 >= arr1->size || index1 < 0)
        return ERROR_MEMORY;
    if (arr2->size + (index_2 - index1) + 1 >= arr2->capasity)
    {
        if (change_capasity(arr2, arr2->capasity + (index_2 - index1) + 1) != OK)
        {
            return ERROR_MEMORY;
        }
    }
    for (int m = index1; m <= index_2; m++)
    {
        int num = arr1->array[m];
        arr2->array[arr2->size] = num;
        arr2->size++;
    }
    return OK;
}
int compareUp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
int compareDown(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a);
}
enum checker fSort(char *str, mas **all_mass)
{
    if (strlen(str) > 2 || strlen(str) < 2)
        return INVALID;
    if (!isalpha(str[0]))
        return INVALID;
    if (str[1] != '+' && str[1] != '-')
        return INVALID;
    char name_mas = toupper(str[0]);
    mas *arr = all_mass[(int)name_mas - 65];
    if (str[1] == '+')
    {
        qsort(arr->array, arr->size, sizeof(int), compareUp);
    }
    else
        qsort(arr->array, arr->size, sizeof(int), compareDown);
    return OK;
}
enum checker fShuffle(char *str, mas **all_mass)
{
    if (strlen(str) != 1 || !isalpha(str[0]))
        return INVALID;
    mas *arr = all_mass[(int)(toupper(str[0]) - 65)];
    qsort(arr->array, arr->size, sizeof(int), compareUp);
    for (int i = arr->size - 2; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arr->array[i];
        arr->array[i] = arr->array[j];
        arr->array[j] = temp;
    }
    return OK;
}
double find_max_away(int *array, int size, double middle_value)
{
    double max_away = 0;
    for (int i = 0; i < size; i++)
    {
        if (abs(array[i] - middle_value) > max_away)
            max_away = array[i] - middle_value;
    }
    return max_away;
}
enum checker fStats(char *str, mas **all_mass, int *size_mas, int *max_min, int *max_min_in, int *most_met, double *middle_value, double *max_away)
{
    if (strlen(str) < 1 || !isalpha(str[0]) || strlen(str) > 2)
        return INVALID;
    mas *arr = all_mass[(int)(toupper(str[0]) - 65)];
    if (arr->size == 0)
        return INVALID;
    *size_mas = arr->size;
    int(*count)[2] = malloc(sizeof(int[2]) * arr->size); // 0 - count, 1-value
    int index_count = 0;
    if (!count)
    {
        return ERROR_MEMORY;
    }
    int max_met = 0;
    *middle_value = 0;
    max_min[0] = INT_MAX;
    max_min[1] = INT_MIN;
    for (int i = 0; i < arr->size; i++)
    {
        *middle_value += arr->array[i];
        int fl = 0;
        for (int m = 0; m < index_count && !fl; m++)
        {
            if (count[m][1] == arr->array[i])
            {
                count[m][0]++;
                if (count[m][0] > max_met)
                {
                    max_met = count[m][0];
                    *most_met = count[m][1];
                }
                if (count[m][0] == max_met && count[m][1] > *most_met)
                {
                    *most_met = count[m][1];
                }
                fl = 1;
            }
        }
        if (!fl)
        {
            count[index_count][0] = 1;
            count[index_count][1] = arr->array[i];
            if (count[index_count][0] > max_met)
            {
                max_met = count[index_count][0];
                *most_met = count[index_count][1];
            }
            if (count[index_count][0] == max_met && count[index_count][1] > *most_met)
            {
                *most_met = count[index_count][1];
            }
            index_count++;
        }
        if (arr->array[i] > max_min[1])
        {
            max_min[1] = arr->array[i];
            max_min_in[1] = i;
        }
        if (arr->array[i] < max_min[0])
        {
            max_min[0] = arr->array[i];
            max_min_in[0] = i;
        }
    }
    *middle_value /= arr->size;
    *max_away = find_max_away(arr->array, arr->size, *middle_value);
    free(count);
    return OK;
}
enum checker fPrintf(char *str, mas **all_mass)
{ // a, 3
    if (!isalpha(str[0]))
        return INVALID;
    char name_mas = toupper(str[0]);
    if (str[1] != ',')
        return INVALID;
    mas *arr = all_mass[(int)name_mas - 65];
    char first_num[4];
    char second_num[4];
    char *str_after_print = &(str[3]);
    int i = 0;
    if (strncmp(str_after_print, "all", 3) == 0)
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
        if (index < 0 || index >= arr->size)
            return ERROR_MEMORY;
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
        if (index_2 < index_1 || index_2 >= arr->size || index_1 < 0)
            return ERROR_MEMORY;
        for (int m = index_1; m <= index_2; m++)
        {
            printf("%d ", arr->array[m]);
        }
        return OK;
    }
}
enum checker fFree(char *str, mas **all_mass)
{
    if (strlen(str) != 2 || !isalpha(str[0]))
        return INVALID;
    mas *arr = all_mass[(int)(toupper(str[0])) - 65];
    if (arr->array != NULL)
        free(arr->array);
    arr->array = NULL;
    arr->capasity = 0;
    arr->size = 0;
    return OK;
}
void free_all(mas **all_mass)
{
    for (int i = 0; i < 26; i++)
    {
        if (all_mass[i]->array != NULL)
        {
            free(all_mass[i]->array);
            all_mass[i]->capasity = 0;
            all_mass[i]->size = 0;
        }
        if (all_mass[i] != NULL)
            free(all_mass[i]);
    }
}

int main(int argc, char*argv[])
{
    mas *all_mass[26];
    char user_str[20];
    for (int i = 65; i < 91; i++)
    {
        all_mass[i - 65] = create_mas(rand() % 20, (char)i);
        if (all_mass[i - 65] == NULL)
        {
            printf("Ошибка памяти\n");
            free_all(all_mass);
            return 1;
        }
    }
    int is_file = 0;
    FILE *file;
    int k = 0;
    if (argc > 1)
    {
        if (!(file = fopen(argv[1], "r")))
        {
            printf("Не удалось открыть входной файл\n");
            return 1;
        }
        is_file = 1;
    }
    if (is_file)
    {
        char c = fgetc(file);
        while (c != '\n' && c != EOF)
        {
            user_str[k] = c;
            c = fgetc(file);
            k++;
        }
        user_str[k] = '\0';
    }
    else
    {
        printf("Введите инструкцию, print или exit для выхода\n");
        char c;
        while ((c = getchar()) != '\n')
        {
            user_str[k] = c;
            k++;
        }
        user_str[k] = '\0';
    }
    while (strcmp(user_str, "exit") != 0 && (strlen(user_str) > 0))
    {
        char instruction[7];
        int i = 0;
        while (user_str[i] != ' ' && user_str[i] != '(')
        {
            instruction[i] = user_str[i];
            i++;
        }
        int index_dop_info = i + 1;
        instruction[i] = '\0';
        if (strcmp(instruction, "Load") == 0) // 10
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
        { // Save A, out.txt; - выгрузить элементы массива A в файл out.txt;
            enum checker ans = fSave(&(user_str[index_dop_info]), all_mass);
            if (ans == INVALID)
            {
                printf("Ошибка\n");
            }
            else if (ans == OK)
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Rand") == 0)
        {
            enum checker ans = fRand(&(user_str[index_dop_info]), all_mass);
            if (ans == INVALID)
            {
                printf("Инвалидные входные данные\n");
            }
            else if (ans == OK)
            {
                printf("Успешно\n");
            }
            else
            {
                printf("Ошибка времени исполнения\n");
                free_all(all_mass);
                return 1;
            }
        }
        else if (strcmp(instruction, "Concat") == 0)
        {
            enum checker ans = fConcat(&(user_str[index_dop_info]), all_mass);
            if (ans == INVALID)
            {
                printf("Инвалидные входные данные\n");
            }
            else if (ans == OK)
            {
                printf("Успешно\n");
            }
            else
            {
                printf("Ошибка времени исполнения\n");
                free_all(all_mass);
                return 1;
            }
        }
        else if (strcmp(instruction, "Remove") == 0)
        {
            enum checker ans = fRemove(&(user_str[index_dop_info]), all_mass);
            if (ans == INVALID)
            {
                printf("Инвалидные входные данные\n");
            }
            else if (ans == OK)
            {
                printf("Успешно\n");
            }
            else
            {
                printf("Ошибка времени исполнения\n");
                free_all(all_mass);
                return 1;
            }
        }
        else if (strcmp(instruction, "Print") == 0)
        {
            if (fPrintf(&(user_str[index_dop_info]), all_mass) == ERROR_MEMORY)
            {
                printf("Инвалидные входные данные\n");
                free_all(all_mass);
                return 1;
            }
        }
        else if (strcmp(instruction, "Copy") == 0)
        {
            enum checker ans = fCopy(&(user_str[index_dop_info]), all_mass);
            if (ans == INVALID)
            {
                printf("Инвалидные входные данные\n");
            }
            else if (ans == OK)
            {
                printf("Успешно\n");
            }
            else
            {
                printf("Ошибка времени исполнения\n");
                free_all(all_mass);
                return 1;
            }
        }
        else if (strcmp(instruction, "Sort") == 0)
        {
            if (fSort(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Инвалидные входные данные\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Shuffle") == 0)
        {
            if (fShuffle(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Инвалидные входные данные\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        else if (strcmp(instruction, "Stats") == 0)
        {
            int size_mas, max_min_value[2], max_min_index[2], most_met;
            double middle, max_away;
            if (fStats(&(user_str[index_dop_info]), all_mass, &size_mas, max_min_value, max_min_index, &most_met, &middle, &max_away) != OK)
            {
                printf("Инвалидные входные данные\n");
            }
            else
            {
                printf("Размер массива: %d\n", size_mas);
                printf("Макс элемент:%d, его индекс: %d, мин элемент: %d, его индекс: %d\n", max_min_value[1], max_min_index[1], max_min_value[0], max_min_index[0]);
                printf("Наиболее часто встречающийся элемент: %d\n", most_met);
                printf("Среднее значение: %f\n", middle);
                printf("Максимальное отклонение: %f\n", max_away);
            }
        }
        else if (strcmp(instruction, "Free") == 0)
        {
            if (fFree(&(user_str[index_dop_info]), all_mass) != OK)
            {
                printf("Инвалидные входные данные\n");
            }
            else
            {
                printf("Успешно\n");
            }
        }
        k = 0;
        if (is_file)
        {
            char c = fgetc(file);
            int k = 0;
            while (c != '\n' && c != EOF)
            {
                user_str[k] = c;
                c = fgetc(file);
                k++;
            }
            user_str[k] = '\0';
        }
        else
        {
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
    free_all(all_mass);
    printf("ok\n");
    if (is_file) fclose(file);
    return 0;
}
