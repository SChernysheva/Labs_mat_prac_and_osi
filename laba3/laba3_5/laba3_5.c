#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ID 1
#define NAME 2
#define SURNAME 3
#define GROUP 4
struct Student {
    int id;
    char name[256];
    char surname[256];
    char group[256];
    unsigned char* examScores;
}typedef student;
enum checker 
{
    OK,
    ERROR_MEMORY,
    INVALID
};
int toInt(char sym)
{
    if (isalpha(sym))
        return toupper(sym) - 'A' + 10;
    else if (isdigit(sym))
        return sym - '0';
    else
        return 0;
}
double get_middle_mark(char* degrees)
{
    double res = 0;
    for (int i = 0; i < 5; i++)
    {
        res += toInt(degrees[i]);
    }
    return res / 5;
}
double get_middle_mark_all(student* students, int count)
{
    double res = 0;
    for (int i = 0; i < count; i++)
    {
        if (students[i].examScores != NULL)
        {
            res += (get_middle_mark(students[i].examScores));
        }
    }
    return res / count;
}
void print_in_file_best_stud(student* students, int count, double middle_mark, FILE* file)
{
    for (int i = 0; i < count; i++)
    {
        if (students[i].examScores != NULL)
        {
            if (get_middle_mark(students[i].examScores) > middle_mark)
            {
                fprintf(file, "%s %s\n", students[i].name, students[i].surname);
            }
        }
    }
}
enum checker load_info(FILE* in, student** res, int* len)
{
    *len = 10;
    *res = (student*)malloc(sizeof(student) *(*len));
    if (!(*res))
    {
        return ERROR_MEMORY;
    }
    int k = 0;
    fseek(in, 0, SEEK_END);
    int end = ftell(in);
    fseek(in, 0, SEEK_SET);
    if (ftell(in) - end == 0)
    {
        free(*res);
        *len = 0;
        return INVALID;
    }
    while (fscanf(in, "%d %s %s %s", &((*res)[k].id), (*res)[k].name, (*res)[k].surname, (*res)[k].group) != EOF)
    {
        unsigned char score;
        (*res)[k].examScores = (unsigned char*)malloc(5 * sizeof(unsigned char));
        if (!(*res)[k].examScores)
        {
            for (int i = 0; i < k; i++)
            {
                free(res[k]->examScores);
            }
            free(*res);
            return ERROR_MEMORY;
        }
        int current_index = ftell(in);
        fseek(in, current_index + 1, SEEK_SET);
        int i = 0;
        char c = fgetc(in);
        while(i < 5 && c != EOF && !isspace(c)) {
            if (isdigit(c))
            {
                if (toInt(c) > 5) c = 5 + '0'; //если оценка больше макс - оценка = макс
                (*res)[k].examScores[i] = c;
                i++;
            }
            c = fgetc(in);
        }
        if (i < 5)
        {
            for (i; i < 5; i++)
            {
                (*res)[k].examScores[i] = '2'; // если кол-во оценок меньше пяти - заполняем оставшиеся двойками
            }
        }
        k++;
        if (k == (*len) )
        {
            *res = (student*)realloc(*res, sizeof(student) * (*len) * 2 + 1);
            if (!(*res))
            {
                free(*res);
                return ERROR_MEMORY;
            }
            (*len) *= 2;
            (*len)++;
        }
    }
    if (k < *len)
    {
        *res = (student*)realloc(*res, sizeof(student) * k);
    }
    (*res)[k].id = -1;
    *len = k;
    return OK;
}
student search_by_id(int id, student* res)
{
    int k = 0;
    while (res[k].id != -1)
    {
        if (res[k].id == id) return res[k];
        k++;
    }
    student stud;
    stud.id = -1;
    return stud;
}
student* search_by_name(char* name, student* stud, int* count)
{
    int len = 10;
    student* res = (student*)malloc(sizeof(student) * len);
    if (!res)
    {
        *count = 0;
        return NULL;
    }
    int i = 0;
    int k = 0;
    while (stud[k].id != -1)
    {
        if (strcmp(stud[k].name, name) == 0) 
        {
            if (i == len)
            {
                res = (student*)realloc(res, sizeof(student) * len * 2);
                if (!res)
                {
                    free(res);
                    *count = 0;
                    return NULL;
                }
                len *= 2;
            }
            res[i] = stud[k];
            i++;
        }
        k++;
    }
    if (i == 0)
    {
        free(res);
        *count = 0;
        return NULL;
    }
    if (i < len)
    {
        res = (student*)realloc(res, sizeof(student) * i);
        len = i;
        res[i].id = -1;
    }
    *count = len;
    return res;
}
student* search_by_surname(char* surname, student* stud, int* count)
{
    int len = 10;
    student* res = (student*)malloc(sizeof(student) * len);
    if (!res)
    {
        *count = 0;
        return NULL;
    }
    int i = 0;
    int k = 0;
    while (stud[k].id != -1)
    {
        if (strcmp(stud[k].surname, surname) == 0) 
        {
            if (i == len)
            {
                res = (student*)realloc(res, sizeof(student) * len * 2);
                if (!res)
                {
                    free(res);
                    *count = 0;
                    return NULL;
                }
                len *= 2;
            }
            res[i] = stud[k];
            i++;
        }
        k++;
    }
    if (i == 0)
    {
        free(res);
        *count = 0;
        return NULL;
    }
    if (i < len)
    {
        res = (student*)realloc(res, sizeof(student) * i);
        len = i;
        res[i].id = -1;
    }
    *count = len;
    return res;
}
student* search_by_group(char* group, student* stud, int* count)
{
    int len = 10;
    student* res = (student*)malloc(sizeof(student) * len);
    if (!res)
    {
        *count = 0;
        return NULL;
    }
    int i = 0;
    int k = 0;
    while (stud[k].id != -1)
    {
        if (strcmp(stud[k].group, group) == 0) 
        {
            if (i == len)
            {
                res = (student*)realloc(res, sizeof(student) * len * 2 + 1);
                if (!res)
                {
                    *count = 0;
                    free(res);
                    return NULL;
                }
                len *= 2;
            }
            res[i] = stud[k];
            i++;
        }
        k++;
    }
    if (i == 0)
    {
        free(res);
        *count = 0;
        return NULL;
    }
    if (i < len)
    {
        res = (student*)realloc(res, sizeof(student) * i);
        len = i;
        res[i].id = -1;
    }
    *count = len;
    return res;
}
int comparator_by_id(const void* a, const void* b)
{
    const student* stud1 = (const student*)a;
    const student* stud2 = (const student*)b;
    return stud1->id - stud2->id;
}
int comparator_by_name(const void* a, const void* b)
{
    const student* stud1 = (const student*)a;
    const student* stud2 = (const student*)b;
    return strcmp(stud1->name, stud2->name);
}
int comparator_by_surname(const void* a, const void* b)
{
    const student* stud1 = (const student*)a;
    const student* stud2 = (const student*)b;
    return strcmp(stud1->surname, stud2->surname);
}
int comparator_by_group(const void* a, const void* b)
{
    const student* stud1 = (const student*)a;
    const student* stud2 = (const student*)b;
    return strcmp(stud1->group, stud2->group);
}
void sort_by(int flag, int count, student* students)
{
    switch(flag)
    {
        case 1:
        qsort(students, count, sizeof(student), comparator_by_id);
        break;
        case 2:
        qsort(students, count, sizeof(student), comparator_by_name);
        break;
        case 3:
        qsort(students, count, sizeof(student), comparator_by_surname);
        break;
        case 4:
        qsort(students, count, sizeof(student), comparator_by_group);
        break;

        default:
        break;
    }
}
void free_students(student* students, int count)
{
    if (students == NULL) return;
    int i = 0;
    while (i < count)
    {
        if (students[i].examScores != NULL) free(students[i].examScores);
        students[i].examScores = NULL;
        i++;
    }
    free(students);
    students = NULL;
}
void print_student(student stud)
{
    if (stud.id == -1 || stud.examScores == NULL)
    {
        printf("Такого студента нет\n");
        return;
    }
    printf("%d %s %s %s %s\n", stud.id, stud.name, stud.surname, stud.group, stud.examScores);
}
void print_all_students(student* students, int count)
{
    int i = 0;
    if (students == NULL || students[i].id == -1 || count == 0) 
    {
        printf("Студентов нет\n");
        return;
    }
    while (i < count)
    {
        print_student(students[i]);
        i++;
    }
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Введите файл с записью студентов\n");
        return 1;
    }

    FILE* file_in = fopen(argv[1], "r");
    if (!file_in) {
        printf("Не удалось открыть входной файл.\n");
        return 1;
    }
    FILE* file_out = fopen(argv[2], "w");
    if (!file_out) {
        fclose(file_in);
        printf("Не удалось открыть выходной файл.\n");
        return 1;
    }
    int count_all;
    student* res;
    enum checker load_check = load_info(file_in, &res, &count_all);
    if (load_check == ERROR_MEMORY)
    {
        printf("Ошибка в выделении памяти\n");
        fclose(file_in);
        fclose(file_out);
        return 0;
    }
    if (load_check == INVALID)
    {
        printf("Некорректный ввод данных\n");
        fclose(file_in);
        fclose(file_out);
        return 0;
    }
    
    int flag;
    char c[1];
    printf("Введите 1 если вы хотите вывести всех студентов, 2 если вы хотите их отсортировать, 3 если хотите произвести поиск, 4 - вывести в файл данные студента");
    printf("и его среднюю оценку, 5 - вывести имена и фамилии лучших студентов в файл и любую другую клавишу для выхода\n");
    scanf("%s", c);
    if (!isdigit(c[0]) || strlen(c) > 1) flag = -1;
    else flag = toInt(c[0]);
    while (flag > 0 && flag <= 5 && flag != -1)
    {
        if (flag == 1)
        {
            print_all_students(res, count_all);
        }
        if (flag == 2)
        {
            int flag_sort;
            char s[1];
            printf("По какому признаку отсортировать? 1 - id, 2 - name, 3 - surname, 4 - group\n");
            scanf("%s", s);
            if (isalpha(s[0] || strlen(s) > 1)) flag_sort = 6;
            else flag_sort = toInt(s[0]);
            if ( abs(flag_sort) < 5) 
            {
                sort_by(abs(flag_sort), count_all, res);
                printf("Данные отсортированы!\n");
            }
        }
        if (flag == 3)
        {
            char s[1];
            int flag_search;
            printf("По какому признаку произвести поиск? 1 - id, 2 - name, 3 - surname, 4 - group\n");
            scanf("%s", s);
            if (isdigit(s[0]) && strlen(s) <= 1)
            {
                int id_;
                char *str;
                int count_;
                flag_search = toInt(s[0]);
                printf("%d\n", flag_search);
                if (flag_search >= 1 && flag_search <= 4)
                {
                    switch (flag_search)
                    {
                    case 1:
                        printf("Введите id:\n");
                        char i[20];
                        scanf("%s", i);
                        id_ = atoi(i);
                        print_student(search_by_id(id_, res));
                        break;

                    case 2:
                        printf("Введите имя для поиска:\n");
                        scanf("%s", str);
                        student *stud_name = search_by_name(str, res, &count_);
                        print_all_students(stud_name, count_);
                        free_students(stud_name, count_);
                        break;

                    case 3:
                        printf("Введите фамилию для поиска:\n");
                        scanf("%s", str);
                        student *stud_surname = search_by_surname(str, res, &count_);
                        print_all_students(stud_surname, count_);
                        free_students(stud_surname, count_);
                        break;

                    case 4:
                        printf("Введите группу для поиска:\n");
                        scanf("%s", str);
                        student *stud_group = search_by_group(str, res, &count_);
                        print_all_students(stud_group, count_);
                        free_students(stud_group, count_);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
        if (flag == 4)
        {
            int id_;
            printf("Введите id:\n");
            char i[20];
            scanf("%s", i);
            id_ = atoi(i);
            student stud = search_by_id(id_, res);
            if (stud.id == -1)
            {
                printf("Такого студента нет\n");
            }
            else 
            {
                double middle_degree;
                if (stud.examScores != NULL) 
                {
                    middle_degree = get_middle_mark(stud.examScores);
                    fprintf(file_out, "%s %s %s %f\n", stud.name, stud.surname, stud.group, middle_degree);
                    printf("Данные записаны в выходной файл!\n");
                }
                else
                {
                    fprintf(file_out, "%s %s %s\n", stud.name, stud.surname, stud.group);
                    printf("Данные студента без среденей оценки записаны в выходной файл!\n");
                }
            }
        }
        if (flag == 5)
        {
            double mid_m = get_middle_mark_all(res, count_all);
            print_in_file_best_stud(res, count_all, mid_m, file_out);
            printf("Записано!\n");
        }
        printf("Введите 1 если вы хотите вывести всех студентов, 2 если вы хотите их отсортировать, 3 если хотите произвести поиск, 4 - вывести в файл данные студента");
        printf("и его среднюю оценку, 5 для вывода лучших студентов в файл и любую другую клавишу для выхода\n");
        scanf("%s", c);
        if (!isdigit(c[0]) || strlen(c) > 1)
            flag = -1;
        else
            flag = toInt(c[0]);
    }
    fclose(file_in);
    fclose(file_out);
    free_students(res, count_all);
    return 0;
}
