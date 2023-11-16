#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define NAME 100
#define SURNAME 200
#define PATRONOMIC 300
#define BIRTHDAY 400
#define BITHMOUNTH 500
#define BIRTHYEAR 600
#define GENDER 700
#define INCOME 800
#define ADD 1
#define DELETE 2
#define CHANGE 3
enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};
struct String {
    char* str;
    int length;
}typedef string;

typedef struct Liver {
    string* surname;
    string* name;
    string* patronymic;
    int birthDay;
    int birthMonth;
    int birthYear;
    string* gender;
    float income;
    struct Liver* next;
} Liver;

typedef struct Data {
    string* surname;
    string* name;
    string* patronymic;
    int birthDay;
    int birthMonth;
    int birthYear;
    string* gender;
    float income;
    Liver* liver;
    int flag;
    int what_was_change;
} data;


typedef struct Stack {
    data** stackArray;
    int capacity;
    int top;
} Stack;

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->stackArray = (data**)malloc(capacity * sizeof(data*));
    if (!stack->stackArray) return NULL;
    return stack;
}
int is_str(char* str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}
int isInt(char* str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}
int isFloat(char* str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]) && str[i] != '.') return 0;
    }
    return 1;
}
void my_free(char* str)
{
    if (str != NULL)
    {
        free(str);
    }
    else return;
    str = NULL;
}
string* make_string(char* str)
{
    string* res = malloc(sizeof(string));
    if (str == NULL || res == NULL)
    {
        res->str = NULL;
        res->length = -1;
        return res;
    }
    int len = strlen(str);
    res->str = (char*)malloc(sizeof(char) * len);
    if (!res->str)
    {
         res->str = NULL;
         res->length = -1;
         return res;
    }
    res->length = len;
    strcpy(res->str, str);
    return res;
}
void delete_str(string* my_str)
{
    if (my_str == NULL || my_str->length == -1) return;
    free(my_str->str);
    my_str->str = NULL;
    my_str->length = -1;
    free(my_str);
    my_str = NULL;
}
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, data* data) {
    stack->stackArray[stack->top + 1] = data;
    (stack->top)++;
}

data* pop(Stack* stack, int* index) {
    if (isEmpty(stack)) {
        return NULL;
    }
    data* data = stack->stackArray[stack->top];
    *index = stack->top;
    (stack->top)--;
    return data;
}

data* peek(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->stackArray[stack->top];
}
void free_liver(Liver* liver)
{
    if (liver == NULL) return;
    delete_str(liver->name);
    delete_str(liver->surname);
    delete_str(liver->patronymic);
    delete_str(liver->gender);
    free(liver);
    liver = NULL;
}
void destroyStack(Stack* stack) {
    if (stack == NULL) {
        return;
    }
    for (int i = 0; i <= stack->top; i++)
    {
        delete_str(stack->stackArray[i]->name);
        delete_str(stack->stackArray[i]->surname);
        delete_str(stack->stackArray[i]->patronymic);
        delete_str(stack->stackArray[i]->gender);
        printf("%d str ok\n", i);
        if (stack->stackArray[i]->flag == DELETE && stack->stackArray[i]->liver != NULL)
        {
            printf("in liver str\n");
            delete_str(stack->stackArray[i]->liver->surname);
            delete_str(stack->stackArray[i]->liver->name);
            delete_str(stack->stackArray[i]->liver->patronymic);
            delete_str(stack->stackArray[i]->liver->gender);
            printf("%d liver str ok\n", i);
            free(stack->stackArray[i]->liver);
            stack->stackArray[i]->liver = NULL;
        }
        printf("%d liver ok\n", i);
        free(stack->stackArray[i]);
    }
    printf("stack top ok\n");
    for (int i = stack->top + 1; i < stack->capacity; i++)
    {
        if (stack->stackArray[i] != NULL) free(stack->stackArray[i]);
    }
    printf("stack capasity ok\n");
    free(stack->stackArray);
    stack->stackArray = NULL;
    free(stack);
    stack = NULL;
}

Stack* createStackWithCopy(Stack* oldStack, int n) {
    int newSize = oldStack->capacity + 1;
    Stack* newStack = createStack(oldStack->capacity + 1);

    int startIndex = oldStack->top - n/2 - 1;
    if (startIndex < 0) startIndex = 0;

    for (int i = startIndex; i < oldStack->top; i++) {
        push(newStack, oldStack->stackArray[i]);
    }
    newStack->top = oldStack->top;
    return newStack;
}

Liver* createLiver(char* surname, char* name, char* patronymic, int birthDay, int birthMonth, int birthYear, char* gender, float income) {
    Liver* liver = (Liver*)malloc(sizeof(Liver));
    if (!liver)
    {
        return NULL;
    }
    liver->surname = make_string(surname);
    if (!liver->surname->str)
    {
        return NULL;
    }
    liver->name = make_string(name);
    if (!liver->name->str)
    {
        delete_str(liver->surname);
        return NULL;
    }
    liver->patronymic = make_string(patronymic);
    if (!liver->patronymic->str)
    {
        delete_str(liver->surname);
        delete_str(liver->name);
        return NULL;
    }
    liver->gender = make_string(gender);
    if (!liver->gender->str)
    {
        delete_str(liver->surname);
        delete_str(liver->name);
        delete_str(liver->patronymic);
        return NULL;
    }
    liver->birthDay = birthDay;
    liver->birthMonth = birthMonth;
    liver->birthYear = birthYear;
    liver->income = income;
    liver->next = NULL;
    return liver;
}
void printLiver(Liver* liver) {
    if (liver == NULL) return;
    printf("%s %s %s %d/%d/%d %s %.2f\n", liver->surname->str, liver->name->str, liver->patronymic->str, liver->birthDay, liver->birthMonth, liver->birthYear, liver->gender->str, liver->income);
}
void print_all(Liver* head)
{
    Liver* current = head;
    while (current != NULL) {
        printLiver(current);
        current = current->next;
    }
    printf("\n");
}
void insertLiver(Liver** head, Liver* newLiver) {
    Liver* current;
    if (*head == NULL || (*head)->birthYear > newLiver->birthYear ||
        ((*head)->birthYear == newLiver->birthYear && (*head)->birthMonth > newLiver->birthMonth) ||
        ((*head)->birthYear == newLiver->birthYear && (*head)->birthMonth == newLiver->birthMonth && (*head)->birthDay > newLiver->birthDay)) {
        newLiver->next = *head;
        *head = newLiver;
    }
    else {
        current = *head;
        while (current->next != NULL &&
            (current->next->birthYear < newLiver->birthYear ||
                (current->next->birthYear == newLiver->birthYear && current->next->birthMonth < newLiver->birthMonth) ||
                (current->next->birthYear == newLiver->birthYear && current->next->birthMonth == newLiver->birthMonth && current->next->birthDay < newLiver->birthDay))) {
                current = current->next;
        }
        newLiver->next = current->next;
        current->next = newLiver;
    }
}

void deleteLiver(Liver** head, const char* surname, const char* name) {
    Liver* current = *head;
    Liver* previous = NULL;
    printf("%s %s\n", surname, name);
    while (current != NULL) {
        if (strcmp(current->surname->str, surname) == 0 && strcmp(current->name->str, name) == 0) {
            if (previous == NULL) {
                *head = current->next;
            }
            else {
                previous->next = current->next;
            }
            delete_str(current->surname);
            delete_str(current->name);
            delete_str(current->patronymic);
            delete_str(current->gender);
            free(current);
            current = NULL;
            return;
        }
        previous = current;
        current = current->next;
    }
}


void freeList(Liver* head) { 
    Liver* current = head;
    while (current != NULL) {
        Liver* temp = current;
        current = current->next;
        delete_str(temp->surname);
        temp->surname->str = NULL;
        temp->surname = NULL;
        delete_str(temp->name);
        temp->name->str = NULL;
        temp->name = NULL;
        delete_str(temp->patronymic);
        temp->patronymic->str = NULL;
        temp->patronymic = NULL;
        delete_str(temp->gender);
        temp->gender->str = NULL;
        temp->gender = NULL;
        free(temp);
        temp = NULL;
    }
}

Liver* findLiver(Liver* head, const char* surname, const char* name) {
    Liver* current = head;
    while (current != NULL) {
        if (strcmp(current->surname->str, surname) == 0 && strcmp(current->name->str, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void updateLiver(Liver** liver, Liver** head, int code, int count, ...) {
    va_list(args);
    va_start(args, count);
    if (code == NAME)
    {
        char* str = va_arg(args, char*);
        delete_str((*liver)->name);
        (*liver)->name = make_string(str);
    }
    else if (code == SURNAME)
    {
        char* str = va_arg(args, char*);
        delete_str((*liver)->surname);
        (*liver)->surname = make_string(str);
    }
    else if (code == PATRONOMIC)
    {
        char* str = va_arg(args, char*);
        delete_str((*liver)->patronymic);
        (*liver)->patronymic = make_string(str);
    }
    else if (code == BIRTHDAY)
    {
        int arg = va_arg(args, int);
        (*liver)->birthDay = arg;
    }
    else if (code == BIRTHYEAR)
    {
        //при изменении даты рождения ливера мы отправляем год в эту функцию в последнюю очередь и пересобираем его чтобы потом переставить в список 
        // год мы отправляем в любом случае даже если поменялось только число рождения ( в мейне будет видно)
        int arg = va_arg(args, int);
        //
        Liver* temp = createLiver((*liver)->surname->str, (*liver)->name->str, (*liver)->patronymic->str, (*liver)->birthDay, (*liver)->birthMonth, arg, (*liver)->gender->str, (*liver)->income);
        deleteLiver(head, (*liver)->surname->str, (*liver)->name->str);
        insertLiver(head, temp);
        *liver = temp;
    }
    else if (code == BITHMOUNTH)
    {
        int arg = va_arg(args, int);
        (*liver)->birthMonth = arg;
    }
    else if (code == GENDER)
    {
        char* str = va_arg(args, char*);
        delete_str((*liver)->gender);
        (*liver)->gender = make_string(str);
    }
    else if (code == INCOME)
    {
        float arg = va_arg(args, double);
        (*liver)->income = arg;
    }
    va_end(args);
}
void undo(Stack* stack, Liver** head)
{
    int index;
    data* info = pop(stack, &index);
    printf("%d\n", info->flag);
    if (info->flag == ADD)
    {
        Liver* lived_adding = info->liver;
        deleteLiver(head, info->liver->surname->str, info->liver->name->str);
    }
    else if (info->flag == DELETE)
    {
        insertLiver(head, info->liver);
    }
    else if (info->flag == CHANGE)
    {
        Liver* changing = info->liver; //апдейт текущего 
        switch (info->what_was_change)
        {
            case NAME:
            updateLiver(&changing, head, NAME, 1, info->name->str);
            delete_str(info->name);
            break;
            case SURNAME:
            updateLiver(&changing, head,  SURNAME, 1, info->surname->str);
            delete_str(info->surname);
            break;
            case PATRONOMIC:
            updateLiver(&changing, head,  PATRONOMIC, 1, info->patronymic->str);
            delete_str(info->patronymic);
            break;
            case BIRTHDAY:
            updateLiver(&changing, head,  BIRTHDAY, 1, info->birthDay);
            updateLiver(&changing, head,  BIRTHYEAR, 1, info->birthYear);
            break;
            case BITHMOUNTH:
            updateLiver(&changing, head,  BITHMOUNTH, 1, info->birthMonth);
            updateLiver(&changing, head,  BIRTHYEAR, 1, info->birthYear);
            break;
            case BIRTHYEAR:
            updateLiver(&changing, head,  BIRTHYEAR, 1, info->birthYear);  
            break;
            case GENDER:
            updateLiver(&changing, head,  GENDER, 1, info->gender->str);
            delete_str(info->gender);
            break;
            case INCOME:
            updateLiver(&info->liver, head,  INCOME, 1, info->income);
            break;
            default:
            break;
        }
    }
    free(stack->stackArray[index]);
    stack->stackArray[index] = NULL;
    info = NULL;
}


int main() {

    FILE* file = fopen("test.txt", "r");
    if (file == NULL) {
        printf("Не удалось открыть файл\n");
        return 1;
    }
    int birthDay, birthMonth, birthYear;
    float income;
    Liver* head = NULL;
    char line[200];
    while (fgets(line, sizeof(line), file) != NULL)  {
        char temp1[50], temp2[50], temp3[50];
        char surname[50], name[50], patronymic[50], gender[1];
        sscanf(line, "%s %s %s", temp1, temp2, temp3);
        if (strlen(temp3) > 2)  //если отчество есть
        {
            sscanf(&(line[0]), "%s %s %s %d %d %d %s %f", surname, name, patronymic, &birthDay, &birthMonth, &birthYear, gender, &income);
        }
        else
        {
            patronymic[0] = '_';
            patronymic[1] = '\0';
            sscanf(&(line[0]), "%s %s %d %d %d %s %f", surname, name, &birthDay, &birthMonth, &birthYear, gender, &income);

        }
        gender[1] = '\0';
        if (!is_str(surname) || !is_str(name) || (!is_str(patronymic) && patronymic[0] != '_') || !is_str(gender) || strlen(gender) > 1 || birthDay > 31 || birthDay < 1 /
        birthMonth > 12 || birthMonth < 1 || birthYear > 2023 || birthYear < 1900 || income < 0 || (gender[0] != 'M' && gender[0] != 'W'))
            {
               printf("Некорректные входные данные\n");
               fclose(file);
               return 0; 
            }
        Liver* newLiver = createLiver(surname, name, patronymic, birthDay, birthMonth, birthYear, gender, income);
        insertLiver(&head, newLiver);
    }
    int capasity = 10;
    Stack* stack = createStack(capasity);
    char user_str[10];
    int count_undo = 0;
    int count_op = 0;
    while(strcmp(user_str, "exit") != 0)
    {
        //if (count_op % 2 == 0 && strcmp(user_str, "UNDO") != 0 && count_undo > 0) count_undo--;
        printf("op: %d, undo: %d %d\n", count_op, count_undo, stack->top);
        if (stack->top >= capasity)
        {
            Stack* new_stack = createStack(stack->capacity * 2);
            if (new_stack == NULL)
            {
                printf("Нехватка памяти\n");
                fclose(file);
                freeList(head);
                destroyStack(stack);
                return 0;
            }
            int start = stack->top - (count_op / 2) - 1;
            printf("%d\n", start);
            if (start < 0)
            {
                start = 0;
            }

            for (int i = start; i < stack->top; i++)
            {
                push(new_stack, stack->stackArray[i]);
            }
            new_stack->top = stack->top - start;
            destroyStack(stack);
            stack = new_stack;
        }
        printf("Введите команду:");
        printf("ADD чтобы добавить жителя, DEL для удаления, UPDATE для изменения, UNDO для отмены последнего действия и exit для выхода\n");
        scanf("%s", user_str);
        if (strcmp(user_str, "print") == 0)
        {
            print_all(head);
        }
        else if (strcmp(user_str, "DEL") == 0)
        {
            data *info = malloc(sizeof(data));
            if (!info)
            {
                printf("Ошибка памяти\n");
                fclose(file);
                freeList(head);
                destroyStack(stack);
                return 0;
            }
            char name[20];
            char surname[20];
            printf("Введите через пробел фамилию и имя жителя для удаления\n");
            scanf("%s %s", surname, name);
            Liver* n = findLiver(head, &(surname[0]), &(name[0]));
            if (n == NULL)
            {
                printf("Такого жителя нет\n");
            }
            else
            {
                Liver* l = createLiver(n->surname->str, n->name->str, n->patronymic->str, n->birthDay, n->birthMonth, n->birthYear, n->gender->str, n->income);
                info->liver = l;
                if (info->liver == NULL)
                {
                    printf("Нехватка памяти\n");
                    fclose(file);
                    freeList(head);
                    destroyStack(stack);
                    return 0;
                }
                info->flag = DELETE;
                info->gender = NULL;
                info->name = NULL;
                info->surname = NULL;
                info->patronymic = NULL;
                push(stack, info);
                deleteLiver(&head, n->surname->str, n->name->str);
                count_op++;
                printf("Удалено\n");
            }
        }
        else if (strcmp(user_str, "ADD") == 0)
        {
            data *info = malloc(sizeof(data));
            if (!info)
            {
                printf("Ошибка памяти\n");
                fclose(file);
                freeList(head);
                destroyStack(stack);
                return 0;
            }
            char birthDay[10], birthMonth[10], birthYear[10];
            char income[10];  
            char surname[50], name[50], patronymic[50], gender[1]; 
            printf("Введите через пробел фамилию, имя и отчество(если его нет введите '_')\n");
            scanf("%s %s %s", surname, name, patronymic);
            while (!is_str(surname) || !is_str(name) || (!is_str(patronymic) && patronymic[0] != '_') || strlen(surname) < 2 || strlen(name) < 2)
            {
                printf("Инвалидный ввод\n");
                printf("Введите через пробел фамилию, имя и отчество(если его нет введите '_')\n");
                scanf("%s %s %s", surname, name, patronymic);
            }
            printf("Также введите его день, месяц и год рождения\n");
            scanf("%s %s %s", birthDay, birthMonth, birthYear);
            while (!isInt(birthDay) || !isInt(birthMonth) || !isInt(birthYear) || strlen(birthYear) < 4 || atoi(birthDay) > 31 || atoi(birthDay) < 1  \
            || atoi(birthMonth) > 12 || atoi(birthMonth) < 1 || atoi(birthYear) > 2023 || atoi(birthYear) < 1900)
            {
                printf("%d %d %d\n", atoi(birthDay), atoi(birthMonth), atoi(birthYear));
                printf("Инвалидный ввод\n");
                printf("Также введите его день, месяц и год рождения\n");
                scanf("%s %s %s", birthDay, birthMonth, birthYear);
            }
            printf("Зарплата равна = \n");
            scanf("%s", income);
            while ( !isFloat(income) || atof(income) < 0 )
            {
                printf("Инвалидный ввод\n");
                printf("Зарплата равна = \n");
                scanf("%s", income);
            }
            printf("Пол жителя: (M или W)\n");
            scanf("%s", gender);
            while(!is_str(gender) || strlen(gender) > 1  ||  (gender[0] != 'M' && gender[0] != 'W'))
            {
                printf("Инвалидный ввод\n");
                printf("Пол жителя: (M или W)\n");
                scanf("%s", gender);
            }
            
            Liver* new_liver = createLiver(surname, name, patronymic, atoi(birthDay), atoi(birthMonth), atoi(birthYear), gender, atof(income));
            if (new_liver == NULL)
            {
                    printf("Нехватка памяти\n");
                    fclose(file);
                    freeList(head);
                    destroyStack(stack);
                    return 0;
            }
            info->liver = new_liver;
            info->flag = ADD;
            info->gender = NULL;
            info->name = NULL;
            info->surname = NULL;
            info->patronymic = NULL;
            push(stack, info);
            insertLiver(&head, new_liver);
            count_op++;
            printf("Добавлено\n");
        }
        else if (strcmp(user_str, "UPDATE") == 0)
        {
            data *info = malloc(sizeof(data));
            if (!info)
            {
                    printf("Ошибка памяти\n");
                    fclose(file);
                    freeList(head);
                    destroyStack(stack);
                    return 0;
            }
            char surname[50], name[50];
            printf("Введите фамилию и имя жителя для изменения через пробел\n");
            scanf("%s %s", surname, name);
            Liver* for_cnange = findLiver(head, surname, name);
            if (for_cnange == NULL)
            {
                printf("Не удалось найти такого жителя\n");
            }
            else 
            {
                printf("Житель:\n");
                printLiver(for_cnange);
                int flag;
                printf("1, 2, 3, 4, 5, 6, 7, 8 - для изменения имени, фамилии, отчества, дня, месяца, года рождения, гендера и зарплаты соответсвенно\n");
                scanf("%d", &flag);
                if (flag < 1 || flag > 8)
                {
                    printf("Невалидный ввод\n");
                }
                else 
                {
                    char str[50];
                    int arg;
                    float income;
                    info->flag = CHANGE;
                    info->liver = for_cnange;
                    if (flag == 1 || flag == 2 || flag == 3 || flag == 7)
                    {   
                        printf("Введите строку для изменения\n");
                        scanf("%s", str);
                        if (flag == 2)
                        {
                            info->gender = NULL;
                            info->name = NULL;
                            info->patronymic = NULL;
                            info->what_was_change = SURNAME;
                            info->surname = make_string(for_cnange->surname->str);
                            updateLiver(&for_cnange, &head, SURNAME, 1, str);
                        }
                        if (flag == 1)
                        {
                            info->gender = NULL;
                            info->surname = NULL;
                            info->patronymic = NULL;
                            info->what_was_change = NAME;
                            info->name = make_string(for_cnange->name->str);
                            updateLiver(&for_cnange, &head, NAME, 1, str);
                        }
                        if (flag == 3)
                        {
                            info->gender = NULL;
                            info->name = NULL;
                            info->surname = NULL;
                            info->what_was_change = PATRONOMIC;
                            info->patronymic =  make_string(for_cnange->patronymic->str);
                            updateLiver(&for_cnange, &head, PATRONOMIC, 1, str);
                        }
                        if (flag == 7)
                        {
                            info->name = NULL;
                            info->surname = NULL;
                            info->patronymic = NULL;
                            info->what_was_change = GENDER;
                            info->gender = make_string(for_cnange->gender->str);
                            updateLiver(&for_cnange, &head, GENDER, 1, str);
                        }
                    }
                    if (flag == 4 || flag == 5 || flag == 6)
                    {
                        info->gender = NULL;
                        info->name = NULL;
                        info->surname = NULL;
                        info->patronymic = NULL;
                        printf("На что вы хотите его изменить?\n");
                        scanf("%d", &arg);
                        if (flag == 4)
                        {
                            info->what_was_change = BIRTHDAY;
                            info->birthDay = for_cnange->birthDay;
                            info->birthYear = for_cnange->birthYear;
                            updateLiver(&for_cnange, &head, BIRTHDAY, 1, arg);
                            updateLiver(&for_cnange, &head, BIRTHYEAR, 1, for_cnange->birthYear);
                        }
                        if (flag == 5)
                        {
                            info->what_was_change = BITHMOUNTH;
                            info->birthMonth = for_cnange->birthMonth;
                            info->birthYear = for_cnange->birthYear;
                            updateLiver(&for_cnange, &head, BITHMOUNTH, 1, arg);
                            updateLiver(&for_cnange, &head, BIRTHYEAR, 1, for_cnange->birthYear);
                        }
                        if (flag == 6)
                        {
                            info->what_was_change = BIRTHYEAR;
                            info->birthYear = for_cnange->birthYear;
                            updateLiver(&for_cnange, &head, BIRTHYEAR, 1, arg);
                        }
                    }
                    if (flag == 8)
                    {
                        printf("Введите новую зарплату\n");
                        scanf("%f", &income);
                        info->what_was_change = INCOME;
                        info->income = for_cnange->income;
                        updateLiver(&for_cnange, &head, INCOME, 1, income);
                    }
                    push(stack, info);
                    count_op++;
                    printf("Изменено\n");
                }
            }
        }
        else if(strcmp(user_str, "UNDO") == 0)
        {
            if (count_undo + 1 > count_op / 2 || count_op == 1 || count_op == 0 || stack->stackArray[stack->top] == NULL || stack->top == -1)
            {
                printf("Превышено допустимое кол-во undo для текущего кол-ва операций\n");
            }
            else
            {
                undo(stack, &head);
                count_undo++;
                count_op;
            }
        }
    }
   

    fclose(file);
    freeList(head);
    printf("list ok\n");
    destroyStack(stack);

    return 0;
}

