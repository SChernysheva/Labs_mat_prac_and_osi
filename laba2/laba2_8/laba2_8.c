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
int get_len(char* str)
{
    int count = 0;
    int i = 0;
    while(str[i])
    {
        i++;
        count++;
    }
    return count;
}
int toInt(char sym)
{
    if(isalpha(sym)) return toupper(sym) - 'A' + 10;
    else if(isdigit(sym)) return sym - '0';
    else return 0;
}
int get_num_aschii(char sym)
{
    if(sym > 9) return sym + 'A' - 10;
    else return sym + '0';
}
enum checkAll reverse(char** str) //enuuuuum
{
    int len = get_len(*str);
    char* res = (char*)malloc(sizeof(char) * (len + 1));
    if(res == NULL)
    {
        return ERROR_MEMORY;
    }
    for(int i = 0; i < len; i++)
    {
        res[i] = (*str)[len - i - 1];
    }
    res[len] = '\0';
    *str = res;
    return OK;
}
enum checkAll check_ss(char c, int base) 
{
    if(toInt(c) >= base) return WRONG_SYSTEM;
    return OK;
}
enum checkAll final_summ(int base, char** res, char** to_add)
{
    int len_to_add = get_len(*to_add);
    int len_res = get_len(*res);
    int len_max = len_res > len_to_add ? len_res : len_to_add;

    char* my_memory;
    my_memory = (char*)malloc(sizeof(char) * (len_max + 1));
    if(my_memory == NULL)
    {
        free(*res);
        return ERROR_MEMORY;
    }
    strcpy(my_memory, *res);
    *res = my_memory;
    int ost = 0;
    for(int i = 0; i < len_max; i++)
    {
        if(check_ss((*to_add)[i], base) == WRONG_SYSTEM)
        {
            free(*res);
            return WRONG_SYSTEM;
        }
        int num = toInt((*res)[i]) + toInt((*to_add)[i]);  //999 + 9999 = 41  1в голове 
        (*res)[i] = get_num_aschii((num + ost) % base);
        ost = (num + ost) / base;
    }
    while(ost > 0)
    {
        (*res)[len_max] = get_num_aschii(ost % base);
        len_max++;
        ost /= base;
    }
    (*res)[len_max] = '\0';
    return OK;
}
enum checkAll summ(char** result, int ss, int count, ...)
{
    if(ss < 2 || ss > 36) return WRONG_SYSTEM;

    va_list args;
    va_start(args, count);
    for(int i = 0; i < count; i++)
    {
        char* term = va_arg(args, char*);
        //printf("%s\n", term);
        reverse(&term);
        enum checkAll ans = final_summ(ss, result, &term);
        if(ans == WRONG_SYSTEM)
        {
            return WRONG_SYSTEM;
        }
        else if (ans == ERROR_MEMORY)
        {
            return ERROR_MEMORY;
        }
    }
    reverse(result);
    return OK;
}
int main()
{
    char* res;
    int base = 9;
    enum checkAll ans = summ(&res, base, 3, "1", "8", "2");
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
        int i = 0;
        while (res[i])
        {
            printf("%c", res[i]);
            i++;
        }
        printf("\n");
    }
    return 0;
}