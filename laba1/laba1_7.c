#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
// абсолютный путь или один и тот же файл
enum errors
{
    OK,
    ERROR_MEMORY,
    EMPTY
} errors;
int check_abs(char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '/') return 1;
        i++;
    }
    return 0;
}
int get_name_file(char* str)
{
    int len = strlen(str);
    int i = len;
    while (i > 0)
    {
        if (str[i] == '/') return i + 1;
        i--;
    }
    return -1;
}
enum errors read_input_from_file_into_array(FILE *file, char **buff, int *len, int* size)
{
    *len = 0;
    char c = fgetc(file);
    if (c == EOF)
        return OK;
    while (isspace(c))
    {
        c = fgetc(file);
    }
    while (c != ' ' && c != '\t' && c != '\n' && c != EOF)
    {
        (*buff)[(*len)++] = c;
        if (*len >= *size)
        {
            char *temp = realloc(*buff, *size * 2);
            if (!temp)
            {
                return ERROR_MEMORY;
            }
            *buff = temp;
            *size *= 2;
        }
        c = fgetc(file);
    }
    (*buff)[(*len)] = '\0';
    return OK;
}
void to_four_ss(char** array, int len, short ** result)
{
    short number_base4 = 0;
    for(int i = 0; i < len; ++i)
    {
        if(isalpha((*array)[i]))
        {
            (*array)[i] = (char)tolower((*array)[i]);
        }
        int temp_num = (int)(*array)[i];
        int base10 = 1;
        while(temp_num > 0)
        {
            number_base4 += (temp_num % 4) * base10;
            base10 *= 10;
            temp_num /= 4;
        }
        (*result)[i] = number_base4;
        number_base4 = 0;
    }
}
void convert_to_lowercase(char** array, int len)
{
    for(int i = 0; i < len; ++i)
    {
        if(isalpha((*array)[i]))
        {
            (*array)[i] = (char)tolower((*array)[i]);
        }
    }
}
void convert_to_ascii_base8(char** array, int len, short ** result)
{
    short number_base8 = 0;
    for(int i = 0; i < len; ++i)
    {
        short temp_num = (short)(*array)[i];
        int base10 = 1;
        while(temp_num > 0)
        {
            number_base8 += (temp_num % 8) * base10;
            base10 *= 10;
            temp_num /= 8;
        }
        (*result)[i] = number_base8;
        number_base8 = 0;
    }
}
int main(int argc, char *argv[])
{
    if (argc < 4 || argc > 5)
    {
        printf("Невалидное кол-во аргументов\n");
        return 0;
    }
    if (argv[1][0] != '-' && argv[1][0] != '\'' && strlen(argv[1]) > 2)
    {
        printf("Невалдиный флаг\n");
        return 0;
    }
    char flag = argv[1][1];
    if (flag == 'r')
    {
        if (argc != 5)
        {
            printf("Невалидное кол-во аргументов\n");
            return 0;
        }
        FILE *out = fopen(argv[4], "w");
        if (!out)
        {
            printf("Не удалось открыть выходной файл\n");
            return 0;
        }
        FILE *file1 = fopen(argv[2], "r");
        if (!file1)
        {
            printf("Не удалось открыть файл 1\n");
            fclose(out);
            return 0;
        }
        FILE *file2 = fopen(argv[3], "r");
        if (!file2)
        {
            printf("Не удалось открыть файл 2\n");
            fclose(out);
            fclose(file1);
            return 0;
        }
        if (strcmp(argv[2], argv[4]) == 0 || strcmp(argv[3], argv[4]) == 0)
        {
            printf("Входной и выходной файл не могут быть одинаковыми\n");
            fclose(out);
            fclose(file1);
            fclose(file2);
            return 0;
        }
        char *buff;
        buff = (char *)malloc(101 * sizeof(char));
        int size = 102;
        if (buff == NULL)
        {
            printf("error memory\n");
            fclose(out);
            fclose(file1);
            fclose(file2);
            return 0;
        }
        int len_r = 0;
        while (!feof(file1) && !feof(file2))
        {
            if (read_input_from_file_into_array(file2, &buff, &len_r, &size) != OK)
            {
                printf("Слишком большая длина лексемы\n");
                if (out != NULL)
                    fclose(out);
                if (file2 != NULL)
                    fclose(file2);
                if (file1 != NULL)
                    fclose(file1);
                free(buff);
                return 0;
            }
            if (len_r > 0)
                fprintf(out, "%s ", buff);

            if (read_input_from_file_into_array(file1, &buff, &len_r, &size) != OK)
            {
                printf("Слишком большая длина лексемы\n");
                if (out != NULL)
                    fclose(out);
                if (file2 != NULL)
                    fclose(file2);
                if (file1 != NULL)
                    fclose(file1);
                free(buff);
                return 0;
            }
            if (len_r > 0)
                fprintf(out, "%s ", buff);
        }
        while (!feof(file1))
        {
            if (read_input_from_file_into_array(file1, &buff, &len_r, &size) != OK)
            {
                printf("Слишком большая длина лексемы\n");
                if (out != NULL)
                    fclose(out);
                if (file2 != NULL)
                    fclose(file2);
                if (file1 != NULL)
                    fclose(file1);
                free(buff);
                return 0;
            }
            if (len_r > 0)
                fprintf(out, "%s ", buff);
        }
        while (!feof(file2))
        {
            if (read_input_from_file_into_array(file2, &buff, &len_r, &size) != OK)
            {
                printf("Слишком большая длина лексемы\n");
                if (out != NULL)
                    fclose(out);
                if (file2 != NULL)
                    fclose(file2);
                if (file1 != NULL)
                    fclose(file1);
                free(buff);
                return 0;
            }
            if (len_r > 0)
                fprintf(out, "%s ", buff);
        }
        printf("done\n");
        if (out != NULL)
            fclose(out);
        if (file2 != NULL)
            fclose(file2);
        if (file1 != NULL)
            fclose(file1);
        free(buff);
        return 0;
    }
    if (flag == 'a')
    {
        int index = 0;
        if (argc != 4)
        {
            printf("Ошибка: некорректный ввод\n");
            return 0;
        }
        if (check_abs(argv[2]))
        {
            index = get_name_file(argv[2]);
        }
        FILE *file = fopen(&(argv[2][index]), "r");
        if (!file)
        {
            printf("Не удалось открыть файл\n");
            return 0;
        }
        index = 0;
        if (check_abs(argv[3]))
        {
            index = get_name_file(argv[3]);
        }
        FILE *out = fopen(&(argv[3][index]), "w");
        if (!out)
        {
            printf("Не удалось открыть файл\n");
            fclose(out);
            return 0;
        }
        if (strcmp(argv[2], argv[3]) == 0)
        {
            printf("Входной и выходной файл не могут быть одинаковыми\n");
            fclose(out);
            fclose(file);
            return 0;
        }
        char *buffer;
        short *result;
        buffer = (char *)malloc(101 * sizeof(char));
        int size = 102;
        int size_r = 102;
        result = (short *)malloc(256 * sizeof(short));
        if (buffer == NULL || result == NULL)
        {
            printf("Не удалось выделить память\n");
            if (file != NULL)
                fclose(file);
            if (out != NULL)
                fclose(out);
            free(buffer);
            free(result);
            return 0;
        }
        int len = 0;
        int count_token = 0;
        while (!feof(file))
        {
            if (read_input_from_file_into_array(file, &buffer, &len, &size) != OK)
            {
                printf("Слишком большая длина лексемы\n");
                if (file != NULL)
                    fclose(file);
                if (out != NULL)
                    fclose(out);
                free(result);
                free(buffer);
                return 0;
            }
            count_token++;
            if (len > 0 && count_token % 10 == 0)
            {
                to_four_ss(&buffer, len, &result);
                for (int i = 0; i < len; ++i)
                {
                    fprintf(out, "'%d'", result[i]);
                }
                fprintf(out, " ");
            }
            else if (len > 0 && count_token % 2 == 0)
            {
                convert_to_lowercase(&buffer, len);
                fprintf(out, "%s ", buffer);
            }
            else if (len > 0 && count_token % 5 == 0)
            {
                convert_to_ascii_base8(&buffer, len, &result);
                for (int i = 0; i < len; ++i)
                {
                    fprintf(out, "'%d'", result[i]);
                }
                fprintf(out, " ");
            }
            else
            {
                fprintf(out, "%s ", buffer);
            }
        }
        printf("done\n");
        if (file != NULL)
            fclose(file);
        if (out != NULL)
            fclose(out);
        free(result);
        free(buffer);
        return 0;
    }
    else
    {
        printf("Невалидный флаг\n");
        return 0;
    }
    return 0;
}