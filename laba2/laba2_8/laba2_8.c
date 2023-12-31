#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

enum checkAll
{
    WRONG_SYSTEM,
    ERROR_MEMORY,
    OK
};
struct for_remove_0
{
    int len;
    int index;
};

int checkZeroStr(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] != '0')
            return 1;
        i++;
    }
    return 0;
}
int get_len(char *str)
{
    if (!str)
        return 0;
    int count = 0;
    int i = 0;
    while (str[i])
    {
        i++;
        count++;
    }
    return count;
}
struct for_remove_0 get_len_without_zero(char *str)
{
    struct for_remove_0 ans;
    if (strlen(str) == 1)
    {
        ans.index = 0;
        ans.len = 1;
        return ans;
    }
    int count = 0;
    int i = 0;
    while (str[i] == '0')
    {
        i++;
    }
    int index = i;
    while (str[i])
    {
        count++;
        i++;
    }
    if (count > 0)
    {
        ans.index = index;
        ans.len = count;
    }
    else
    {
        ans.index = 0;
        ans.len = 1;
    }
    return ans;
}
int toInt(char sym)
{
    if (isalpha(sym))
        return toupper(sym) - 'A' + 10;
    else if (isdigit(sym))
        return sym - '0';
    else
        return 0;
}
int get_num_aschii(char sym)
{
    if (sym > 9)
        return sym + 'A' - 10;
    else
        return sym + '0';
}

enum checkAll check_ss(char c, int base)
{
    if (!isalnum(c))
        return WRONG_SYSTEM;
    if (toInt(c) >= base)
        return WRONG_SYSTEM;
    return OK;
}
enum checkAll final_summ(int base, char **res, char **to_add)
{
    int len_to_add = get_len(*to_add);
    for (int i = 0; i < len_to_add; i++)
    {
        if (check_ss((*to_add)[i], base) == WRONG_SYSTEM)
        {
            free(*res);
            return WRONG_SYSTEM;
        }
    }
    int len_res = get_len(*res);
    int len_max = len_res > len_to_add ? len_res : len_to_add;

    char *my_memory;
    my_memory = (char *)malloc(sizeof(char) * (len_max + 2));
    if (my_memory == NULL)
    {
        free(*res);
        return ERROR_MEMORY;
    }
    strcpy(my_memory, *res);
    free(*res);
    *res = my_memory;
    int ost = 0;
    for (int i = len_max; i >= 0; i--) // 10 5
    {
        int a = (len_to_add - 1 >= 0) ? toInt((*to_add)[len_to_add - 1]) : 0;
        int b = (len_res - 1 >= 0) ? toInt((*res)[len_res - 1]) : 0;
        int summ = a + b + ost;
        (*res)[i] = get_num_aschii(summ % base);
        ost = summ / base;
        len_res--;
        len_to_add--;
    }
    struct for_remove_0 ans = get_len_without_zero(*res);
    char *temp = (char *)malloc(sizeof(char) * (ans.len + 1));
    if (!temp)
    {
        free(*res);
        return ERROR_MEMORY;
    }
    int index = ans.index;
    int i = 0;
    while ((*res)[index])
    {
        temp[i] = (*res)[index];
        index++;
        i++;
    }
    free(*res);
    *res = temp;
    (*res)[ans.len + 1] = '\0';
    return OK;
}
enum checkAll summ(char **result, int ss, int count, ...)
{
    if (ss < 2 || ss > 36)
        return WRONG_SYSTEM;
    if (result == NULL)
        return ERROR_MEMORY;
    *result = (char *)malloc(sizeof(char) * 2);
    if (!(*result))
    {
        return ERROR_MEMORY;
    }
    (*result)[0] = '0';
    (*result)[1] = 0;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++)
    {
        char *term = va_arg(args, char *);
        enum checkAll ans = final_summ(ss, result, &term);
        if (ans == WRONG_SYSTEM)
        {
            return WRONG_SYSTEM;
        }
        else if (ans == ERROR_MEMORY)
        {
            return ERROR_MEMORY;
        }
    }
    return OK;
}
int main()
{
    char *res = NULL;
    int base = 16;
    enum checkAll ans = summ(&res, base, 3, "2F", "00001", "10000");
    if (ans == WRONG_SYSTEM)
    {
        printf("Uncorrect args\n");
        return 0;
    }
    else if (ans == ERROR_MEMORY)
    {
        printf("Error memory\n");
        return 0;
    }
    else
    {
        printf("%s", res);
        printf("\n");
        free(res);
    }
    return 0;
}