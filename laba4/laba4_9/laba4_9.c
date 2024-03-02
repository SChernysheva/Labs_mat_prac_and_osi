#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "BinaryHeap.c"
#include "BinomialHeap.c"
#include "dynamicArray.c"
#include "structures.h"
#include "FibonacciHeap.c"
#include "BST.c"
#include "treap.c"
#include "SkewHeap.c"
#define EPS 0.1e17
//binomial, heaps, storages, ident_all    check free, enums
//все точно ок с binaryHeap, fibonacciHeap, ARRAY, BST
enum answer read_app(FILE* file, otd* otds, void* storage, enum heap eHeap, enum storage eStorage, 
                        int* count_apps, double coeff, enum answer (*insertHeap)(void*, application* app), void (*meld_heaps)(void*, void*), otd** least_load, char* ident_ll)
{//void (*meld_heaps)()
    application* app = (application*)malloc(sizeof(application));
    if (!app)
    {
        //todo
    }
    fscanf(file, "%s %d %s", app->time_app, &(app->prior), app->ident);
    app->text = malloc(sizeof(char) * 256);
    if (!app->text)
    {
        return ERROR_MEMORY;
    }
    fgets(app->text, 256, file);
    app->ident_app = *count_apps; //идентифкатор заявки - ее номер по счету
    (*count_apps)++; //это общий счетчик заявок, нужен только для идентифкатора заявки
    void* stor;
    otd* otdd;
    if (eStorage == ARRAY)
    {
        DepartmentArray* stor = (DepartmentArray*)storage;
        otdd = searchDepartmentArray(storage, app->ident);
    }
    else if (eStorage == BST)
    {
        node_BST* stor = (node_BST*)storage;
        //printf("%s\n", stor->key);
        otdd = search_BST(stor, app->ident);
    }
        //otd *otdd = searchOtd(storage, app->ident);  нужны функции insertHeap, meld

        void* heap = otdd->heap;
        if (!heap)
        {
            return INVALID;
        }
        if (insertHeap(heap, app) != ERROR_MEMORY)
        {
            printf("Поступление заявки, идентификатор отделения - %s, время поступления: %s, идентификатор заявки: %d\n", app->ident, app->time_app, app->ident_app);
        }
        otdd->count_real_apps++;
        otdd->count_free_apps++;
        if (otdd->count_op != 0) otdd->coeff_load = (double)otdd->count_real_apps / (double)otdd->count_op; //сразу считаем(пересчитываем) коеф загруженности
        if (!(*least_load)) //если еще нет отделения с самой низкой загруженностью - делаем его
        {
            *least_load = otdd;
            ident_ll = app->ident;
            printf("мин отд %f %s\n", (*least_load)->coeff_load, ident_ll);
        }
        else
        { //иначе сравниваем кефы и если что меняем указатель
            if (otdd->coeff_load < (*least_load)->coeff_load) 
            {
                *least_load = otdd;
                ident_ll = app->ident;
            }
        }
        if (otdd->coeff_load >= coeff) 
        {
            otdd->isOverload = 1;
            printf("Отделение %s перегружено. Перегрузка произошла после заявки с идентифкатором %d\n", app->ident, app->ident_app);
            if (*least_load && *least_load != otdd)
            {
                meld_heaps((*least_load)->heap, otdd->heap);
                printf("Отделение %s передало заявки отделению %s.\n", app->ident, ident_ll);
                otdd->coeff_load = 0; 
                (*least_load)->count_free_apps += otdd->count_free_apps;
                (*least_load)->count_real_apps += otdd->count_real_apps;
                otdd->count_free_apps = 0;
                otdd->count_real_apps = 0;
                (*least_load)->coeff_load = (*least_load)->count_real_apps / (*least_load)->count_op; 
                //передать заявления 
                //пересчитать кэф
            }
        }
}
void read_from_file(FILE* file, enum heap* eHeap, enum storage* eStorage) //читаю из файла какая будет куча и храилище
{
    char heap[25]; 
    char storage[25];
    fscanf(file, "%s", heap);

    if (strcmp(heap, "BinaryHeap") == 0) *eHeap = BINARY;
    else if (strcmp(heap, "BinomialHeap") == 0) *eHeap = BINOMIAL;
    else if (strcmp(heap, "FibonacciHeap") == 0) *eHeap = FIBONACCI;
    else if (strcmp(heap, "SkewHeap") == 0) *eHeap = SKEW;
    else if (strcmp(heap, "LeftistHeap") == 0) *eHeap = LEFTIST;
    else if (strcmp(heap, "TreapHeap") == 0) *eHeap = TREAP;
    //
    fscanf(file, "%s", storage);
    //
    if (strcmp(storage, "HashSet") == 0) *eStorage = HASHSET;
    if (strcmp(storage, "DynamicArray") == 0) *eStorage = ARRAY;
    if (strcmp(storage, "BinarySearchTree") == 0) *eStorage = BST;
    if (strcmp(storage, "Trie") == 0) *eStorage = TRIE;
}
void process_apps(void* stor, enum heap eHeap, otd* otds, int count_otd, int max_time_app, double overload_coeff)
{
    application* (*extract_max)(void*);
    switch (eHeap)
    {
        case BINARY:
        extract_max = &extractMaxBinaryHeap;
        break;

        case BINOMIAL:
        extract_max = &extractMaxBinomial;
        break;

        case FIBONACCI:
        extract_max = &extract_max_fib;
        break;

        case SKEW:
        extract_max = &extract_max_skew;
        break;
    }
    int iter = 0;
    int success = 0;
    while (success != count_otd)
    {
        int i = 0;
        for (i; i < count_otd; i++)
        {
            if (otds[i].isOverload != -1)
            {
                for (int op = 0; op < otds[i].count_op; op++)
                {

                    //printf("отделение %d  оператор номер %d\n", i, op);
                    if (otds[i].operators[op].app && otds[i].operators[op].time_process <= iter - otds[i].operators[op].start_time)
                    { // если оператор был занят и закончил
                        printf("Закончена обработка заявки. Идентификатор %d, оператор %s, время выполнения - %d мин\n",
                               otds[i].operators[op].app->ident_app, otds[i].operators[op].name, otds[i].operators[op].time_process);
                        otds[i].count_real_apps--;
                        if (otds[i].count_real_apps == 0)
                        {
                            otds[i].isOverload = -1;
                            success += 1;
                        }
                        free_application(otds[i].operators[op].app);
                        otds[i].operators[op].app = NULL;
                    }
                    else if (!(otds[i].operators[op].app))
                    {
                        //printf("отделение %d  оператор номер %d\n", i, op);
                        // оператор не занят
                        application *app;
                        if (otds[i].count_free_apps > 0)
                        { // если есть еще заявления
                            app = extract_max(otds[i].heap);
                            otds[i].count_free_apps--;
                            otds[i].operators[op].app = app;
                            otds[i].operators[op].start_time = iter;
                            printf("Начало обработки заявки. Идентификатор заявки - %d, оператор - %s\n", app->ident_app, otds[i].operators[op].name);
                        }
                        else if (otds[i].count_real_apps == 0)
                        {
                            if (otds[i].isOverload != -1) 
                            {
                                success += 1;
                                otds[i].isOverload = -1;
                                printf("+1\n");
                            }
                        }
                    }
                    //else {printf("оператор %s работает\n", otds[i].operators[op].name);}
                    iter += 1;
                }
            }
            else iter += 1;
        }
    }
    
}
void free_storage(enum storage eStor, void* storage, void (*free_heap)(void*, int))
{
    switch(eStor)
    {
        case ARRAY:
        freeDepartmentArray(storage, free_heap);
        break;
        case BST:
        free_BST(storage, free_heap);
        break;
        default: break;
    }
}
enum answer do_heaps(otd* otds, int count, enum heap eHeap, int min_time, int max_time, void* (*create_heap)(void)) //в каждом отделении создаю кучу
{
    for (int i = 0; i < count; i++)
        {
            otds[i].operators = (operator*)malloc(sizeof(operator) * otds[i].count_op );
            if (!(otds[i].operators)) return ERROR_MEMORY;
            for (int k = 0; k < otds[i].count_op; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    otds[i].operators[k].name[l] = 'a' + rand() % 26;
                }
                otds[i].operators[k].name[3] = '\0';
                otds[i].operators[k].start_time = 0;
                otds[i].operators[k].app = NULL;
                otds[i].operators[k].time_process = rand() % (max_time - min_time + 1) + min_time;
            }
           otds[i].heap = create_heap();
           if (!(otds[i].heap)) 
           {
            for (int k = 0; k < i; k++)
            {
                free(otds[i].operators);
            }
            //free heap foo 
            return ERROR_MEMORY;
           }
        }
}
void* do_storage(FILE* file, otd* otds, int count, enum storage eStorage) //создаю выбранное хранилище и вставляю туда пары идентификатор(считывая с файла) - отделение(объект кучи)
{
    if (eStorage == ARRAY)
    {
        DepartmentArray* da = createDepartmentArray(count);
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 4);
            //todo!!
            fscanf(file, "%s", ident_key);
            ident_key[4] = '/0';
            insertDepartmentArray(da, ident_key, &(otds[i]));
            //(otds[i].count_apps)++;
            
        }
        return da;
    }
    else if (eStorage == BST)
    {
        node_BST* bst = NULL;
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 3);
            //todo!!
            fscanf(file, "%s", ident_key);
            if (!bst)  
            {
                bst = createnode_BST(ident_key, &(otds[i]));
            }
            else insert_BST(bst, ident_key, &(otds[i]));
        }
        return bst;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("%s\n", "Недостаточное кол-ыо аргументов\n");
        return 0;
    }
    int max_p = atoi(argv[1]);
    FILE* file1 = fopen(argv[2], "r");
    if (!file1)
    {
        printf("%s\n", "Не удалось открыть входной файл\n");
        return 1;
    }
    enum heap eHeap;
    enum storage eStorage;
    char start[19], end[19];
    int min_time, max_time, count;
    double overload_coeff;
    otd* otds;
    read_from_file(file1, &eHeap, &eStorage);
    fscanf(file1, "%s %s %d %d %d", start, end, &min_time, &max_time, &count);
    otds = (otd*)malloc(sizeof(otd) * count);
    if (!otds)
    {
        fclose(file1);
        printf("Ошибка в выделении памяти\n");
        return 0;
    }
    for (int i = 0; i < count; i++)
    {
        fscanf(file1, "%d", &(otds[i].count_op));
        otds[i].count_real_apps = 0;
        otds[i].count_free_apps = 0;
    }
    enum answer (*foo_insert)(void*, application* app);
    void (*free_heap)(void*, int);
    void (*meld)(void*, void*);
    switch (eHeap)
    {
        case BINARY:
        do_heaps(otds, count, eHeap, min_time, max_time,  (void* (*) (void)) &createBinaryHeap);
        foo_insert = &insertBinaryHeap;
        free_heap = &free_binary_heap;
        meld = &mergeBinaryHeapsWithoutDestruction;
        break;
        case BINOMIAL:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &createBinomaialHeap);
        foo_insert = &insertBinomialHeap;
        free_heap = &freeHeapBinomial;
        //meld
        case FIBONACCI:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &make_fib_heap);
        foo_insert = &fib_heap_insert;
        free_heap = &free_fib_heap;
        meld = &fib_heap_merge_without_destr;
        break;
        case TREAP:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &createTreapNode);
        foo_insert = &insert_treap;
        free_heap = &free_treap;
        meld = &merge_treap_without_destr;
        break;
        case SKEW:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &create_skew);
        foo_insert = &insert_skew;
        meld = &merge_skew;
        free_heap = &free_skew;
    }
    void* stor = do_storage(file1, otds, count, eStorage);
    fscanf(file1, "%lf", &overload_coeff);
    fclose(file1);
    //сейчас имеем хранилище в котором пары идентификатор - отделение, дальше надо обработать заявки 
    //в каждом отделении есть массив чаров на кол-во оператров у каждого из которых свое имя (буква)
    //инфа о хранилище(stor) и отделениях(otds) в енамах eHeap, eStorage
    int i = 3, count_apps = 1;
    FILE* file;
    otd* least_load = NULL;
    char* ident_ll;
    while(file = fopen(argv[i], "r"))
    {
        read_app(file, otds, stor, eHeap, eStorage, &count_apps, overload_coeff, foo_insert, meld, &least_load, ident_ll);
        i++;
        fclose(file);
    }
    process_apps(stor, eHeap, otds, count, max_time, overload_coeff);

    //чистка:
    //вызваать чистку в хранилище(она почистит еще и кучи и операторов)
    free_storage(eStorage, stor, free_heap);
    //почистить память под отделения(просто массив почистить)
    free(otds);
    return 0;
}