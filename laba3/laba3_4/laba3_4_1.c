#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

struct String {
    char* str;
    int length;
}typedef string;

enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};

string make_string(char* str)
{
    string res;
    if (str == NULL)
    {
        res.str = NULL;
        res.length = -1;
        return res;
    }
    int len = strlen(str);
    res.str = (char*)malloc(sizeof(char) * len);
    if (!res.str)
    {
         res.str = NULL;
         res.length = -1;
         return res;
    }
    res.length = len;
    strcpy(res.str, str);
    return res;
}
void delete_str(string* my_str)
{
    free(my_str->str);
    my_str->str = NULL;
    my_str->length = -1;
}
int is_equal(string str1, string str2)
{
    if (str1.str == NULL || str2.str == NULL)
    {
        return -2;
    }
    return (strcmp(str1.str, str2.str) == 0) ? 0 : -1;
}
string copy_in_new(string str1)
{
    string new_str = make_string(str1.str);
    return new_str;
}
enum checker copy_in_exist(string* str1, string str2)
{
    free((*str1).str);
    *str1 = make_string(str2.str);
    if (str1->str == NULL) return ERROR_MEMORY;
    return OK;
}
enum checker concatenate_in_exist(string* str2, string str1)
{
    char res[str1.length + (*str2).length + 1];
    memcpy(res, (*str2).str, (*str2).length);
    memcpy(res + (*str2).length, str1.str, str1.length);
    res[str1.length + (*str2).length + 1] = '\0';
    string str_res = make_string(res);
    if (str_res.str == NULL)
    {
        return ERROR_MEMORY;
    }
    free((*str2).str);
    *str2 = str_res;
    return OK;
}
void print_str(string str1)
{
    if (str1.str == NULL || str1.length < 0)
    {
        printf("Такой строки нет\n");
        return;
    }
    printf("%s\n", str1.str);
}
int compare_strings(string str1, string str2)
{
    //1 большк 0 равно -1 меньше
    if (str1.length > str2.length) return 1;
    else if (str1.length < str2.length) return -1;
    for (int i = 0; i <= str1.length; i++)
    {
        if (str1.str[i] != str2.str[i])
        {
            return (str1.str[i] > str2.str[i]) ? 1 : 0;
        }
    }
    return 0;
    

}
int main()
{
    char* s = "lalalala";
    char* s2 = "11111111";
    char* s3 = "myamya";
    string str1 = make_string(s); //lalalal

    string str2 = copy_in_new(str1); //lalla
    print_str(str2);

    string str3  = make_string(s2);// 11111
    copy_in_exist(&str2, str3); //1111
    print_str(str2);

    concatenate_in_exist(&str3, str1);
    print_str(str3);

    printf("%d\n", compare_strings(str1, str2));
    copy_in_exist(&str1, str2);
    printf("%d\n", compare_strings(str1, str2));

    delete_str(&str1);
    delete_str(&str2);
    delete_str(&str3);

    print_str(str1);
    return 0;
}