#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
typedef struct
{
    int value;
    char *name;
} MemoryCell;

typedef struct
{
    MemoryCell **data;
    int size;
    int capasity;
} list;

typedef enum
{
    OK,
    ERROR_MEMORY,
    ERROR_OPENING_FILE,
    INVALID,
    NULL_,
    NO_SEARCH
} error;
int is_in_int(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}
list *create_list(int capasity)
{
    list *res = malloc(sizeof(list));
    if (!res)
    {
        return NULL;
    }
    res->data = malloc(sizeof(MemoryCell*) * capasity);
    if (!res->data)
    {
        free(res);
        return NULL;
    }
    for (int i = 0; i < capasity; i++)
    {
        res->data[i] = NULL;
    }
    res->capasity = capasity;
    res->size = 0;
    return res;
}
MemoryCell *create_MC(char *name, int value)
{
    MemoryCell *res = malloc(sizeof(MemoryCell));
    if (!res)
    {
        return NULL;
    }
    res->name = strdup(name);
    res->value = value;
    return res;
}
int comparator(const void *mc1, const void *mc2)
{
    const MemoryCell *m1 = *((const MemoryCell **)mc1);
    const MemoryCell *m2 = *((const MemoryCell **)mc2);
    return strcmp(m1->name, m2->name);
}
error insert(list *all_mass, MemoryCell *elem)
{
    if (all_mass->size + 1 >= all_mass->capasity)
    {
        all_mass->capasity *= 2;
        MemoryCell **temp = realloc(all_mass->data, sizeof(MemoryCell*) * all_mass->capasity);
        if (!temp)
        {
            all_mass->capasity /= 2;
            return ERROR_MEMORY;
        }
        all_mass->data = temp;
    }
    all_mass->data[all_mass->size] = elem;
    (all_mass->size)++;
    qsort(all_mass->data, all_mass->size, sizeof(MemoryCell*), comparator);
    return OK;
}
char *get_expression(FILE *file)
{
    int size = 10;
    char *str = (char *)malloc(size);
    if (!str)
    {
        return NULL;
    }
    char c = fgetc(file);
    int i = 0;
    while (c != EOF && c != ';')
    {
        if (i >= size)
        {
            size *= 2;
            char *temp = realloc(str, size);
            if (!temp)
            {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[i] = c;
        i++;
        c = fgetc(file);
    }
    if (i == 0)
    {
        free(str);
        return NULL;
    }
    str[i] = ';';
    str[i + 1] = '\0';
    return str;
}
int BinSearch(list *lists, char *key) // Метод двоичного поиска (дихотомия)
{
    int low = 0;            
    int high = lists->size; 
    int mid;              
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (lists->data[mid] == NULL)
            return -1;   
        MemoryCell* elem = lists->data[mid];                           
        if (strcmp(key, elem->name) == 0) 
            return mid;
        else if (strcmp(key, elem->name) > 0) 
            low = mid + 1;
        else 
            high = mid - 1;
    }
    return -1; 
}
int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%');
}
void print_all(list *lists)
{
    printf("Все переменные:\n");
    for (int i = 0; i < lists->size; i++)
    {
        printf("%s %d\n", lists->data[i]->name, lists->data[i]->value);
    }
}
void print_elem(list* all_data, int index)
{
    printf("Значение переменной %s - %d\n", all_data->data[index]->name, all_data->data[index]->value);
}
error read_one_str(char *str, list *all_data)
{
    char f_part[100];
    char s_part[100];
    char t_part[100];
    int i = 0;
    int in_1 = 0;
    while (!isalnum(str[i]))
        i++;
    while (str[i] != ';' && str[i] != '=')
    {
        f_part[in_1] = str[i];
        i++;
        in_1++;
    }
    f_part[in_1] = '\0';
    int is_exist = 0;
    int index_1 = BinSearch(all_data, f_part);
    if (index_1 != -1)
        is_exist = 1;
    if (str[i] == '=')
    {
        i++;
        int i_2 = 0;
        while (str[i] != ';' && !isOperator(str[i]))
        {
            s_part[i_2] = str[i];
            i++;
            i_2++;
        }
        s_part[i_2] = '\0';
        if (str[i] == ';')
        {
            if (is_in_int(s_part))
            {
                if (is_exist)
                    all_data->data[index_1]->value = atoi(s_part);
                else
                {
                    MemoryCell *new_el = create_MC(f_part, atoi(s_part));
                    if (new_el == NULL)
                    {
                        return ERROR_MEMORY;
                    }
                    if (insert(all_data, new_el) != OK)
                    {
                        return ERROR_MEMORY;
                    }
                    return OK;
                }
                return OK;
            }
            else
            {
                int index_2 = BinSearch(all_data, s_part);
                if (index_2 != -1)
                {
                    if (is_exist)
                    {
                        all_data->data[index_1]->value = all_data->data[index_2]->value;
                        return OK;
                    }
                    else
                    {
                        MemoryCell *new_al = create_MC(f_part, all_data->data[index_2]->value);
                        if (insert(all_data, new_al) != OK)
                        {
                            return ERROR_MEMORY;
                        }
                        return OK;
                    }
                }
                else
                    return INVALID;
            }
        }
        if (isOperator(str[i]))
        {
            char op = str[i];
            i++;
            int i_3 = 0;
            while (str[i] != ';' && str[i] != '\0' && str[i] != EOF)
            {
                t_part[i_3] = str[i];
                i++;
                i_3++;
            }
            t_part[i_3] = '\0';
            int res;
            if (is_in_int(s_part))
            {
                res = atoi(s_part);
            }
            else
            {
                int index_2 = BinSearch(all_data, s_part);
                if (index_2 != -1)
                {
                    res = all_data->data[index_2]->value;
                }
                else
                    return INVALID;
            }
            int t__part;
            if (is_in_int(t_part))
            {
                t__part = atoi(t_part);
            }
            else
            {
                int index_3 = BinSearch(all_data, t_part);
                if (index_3 != -1)
                {
                    t__part = all_data->data[index_3]->value;
                }
                else
                    return INVALID;
            }
            switch (op)
            {
            case '+':
                res += t__part;
                break;
            case '-':
                res -= t__part;
                break;
            case '*':
                res *= t__part;
                break;
            case '/':
                res /= t__part;
                break;
            case '%':
                res %= t__part;
                break;
            default:
                break;
            }
            if (is_exist)
            {
                all_data->data[index_1]->value = res;
                return OK;
            }
            else
            {
                MemoryCell *new_el = create_MC(f_part, res);
                if (new_el == NULL)
                {
                    return ERROR_MEMORY;
                }
                if (insert(all_data, new_el) != OK)
                {
                    return ERROR_MEMORY;
                }
                return OK;
            }
        }
    }
    if (str[i] == ';')
    {
        if (strlen(f_part) == 5)
        {
            print_all(all_data);
            return OK;
        }
        else
        {
            char* name = &(f_part[6]);
            int index = BinSearch(all_data, name);
            if (index == -1)
            {
                return INVALID;
            }
            print_elem(all_data, index);
            return OK;
        }
    }
}
void free_all(list* data)
{
    for (int i = 0; i < data->capasity; i++)
    {
        if (data->data[i] != NULL)
        {
            free(data->data[i]->name);
        }
        free(data->data[i]);
    }
    free(data->data);
    free(data);
}

int main(int argc, char *argv[])
{
    FILE *file;
    if (!(file = fopen(argv[1], "r")))
    {
        printf("Не удалось открыть входной файл\n");
        return 1;
    }
    list *all_data = create_list(10);
    char *line;
    while (line = get_expression(file))
    {
        if (read_one_str(line, all_data) != OK)
        {
            printf("Необъявленная переменная или ошибка памяти\n");
            free_all(all_data);
            free(line);
            fclose(file);
            return 0;
        }
        free(line);
    }
    //print_all(all_data);
    free_all(all_data);
    fclose(file);
    return 0;
}