
#ifndef LALALA
#define LALALA

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

enum answer
{
    OK,
    ERROR_MEMORY,
    INVALID
};
typedef struct Application {
    char time_app[20];
    int prior;
    char ident[4];
    int ident_app;
    char* text;
} application;

typedef struct Timee
{
    int year;
    int mounth;
    int day;
    int hour;
    int min;
    int second;
} my_time;
typedef struct operatorr
{
    int time_process;
    char name[4];
    int start_time; //время в которое он взял крайнюю заявку
    application* app; //заявка которая сейчас в обоаботке
}operator;

typedef struct OTD
{//убрала идетн отделения чтобы хранить его в паре ключ-значение!!!!!!!!
    int count_op; //кол-во операторов
    int count_free_apps; //количество свободных заявок которые можно взять оператору(они не в обработке)
    int count_real_apps; //количество оставшихся заявлений (свободные заявки + заявки которые уже в обработке)
    void* heap; 
    operator* operators; 
    int isOverload; //перегружено ли отделение
    double coeff_load; //коэф загруженности
}otd;


enum heap
{
    BINARY,
    BINOMIAL,
    FIBONACCI,
    SKEW,
    LEFTIST,
    TREAP

};
enum storage
{
    HASHSET,
    ARRAY,
    BST,
    TRIE
};
enum code 
{
    NEW_REQUEST,
    REQUEST_HANDLING_STARTED,
    REQUEST_HANDLING_FINISHED,
    DEPARTMENT_OVERLOADED,

};

void free_application(application* app)
{
    if (!app) return;
    free(app->text);
    free(app);
    app = NULL;
}

typedef struct LogMessage
{
    enum code eCode;
    char* ident_otd;
    int ident_app;
    char* name_op;
    int time_treatment;
} log_msg;

// void printt(DepartmentArray* da) //просто тестф для себя
// {
//     printf("%s\n", da->keys[2]);
// }

// otd* create_otd(int count_op, enum heap eHeap) //создаю отделение и кучу в нем (вроде нигдк не используется)
// {
//     otd* new_otd = (otd*)malloc(sizeof(otd));
//     if (!new_otd)
//     {
//         //TODO
//     }
//     new_otd->count_op = count_op;
//     if (eHeap == BINARY)
//     {
//         new_otd->heap = createBinaryHeap(10);
//     }
//     //TODO
//     return new_otd;
// }
#endif