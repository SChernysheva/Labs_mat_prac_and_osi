#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "BinaryHeap.c"
#include "BinomialHeap.c"
#include "DynamicArray.c"
#include "structures.h"
#include "FibonacciHeap.c"
#include "BST.c"
#include "Treap.c"
#include "SkewHeap.c"
#include "HashSet.c"
#include "LeftistHeap.c"
#include "control_time.c"
#include "trie.c"
#define EPS 0.1e17
void log_(FILE* file, log_msg* msg, int* seek, my_time* tm)
{

    fprint_time(file, tm, seek);

    fseek(file, *seek, SEEK_SET);
    switch(msg->eCode)
    {
    case NEW_REQUEST:
    *seek += fprintf(file, "%s", "[NEW_REQUEST]: ");
    *seek += fprintf(file, "Поступление заявки, идентификатор отделения - %s, идентификатор заявки: %d\n", msg->ident_otd, msg->ident_app);

    break;
    
    case REQUEST_HANDLING_STARTED:
    *seek += fprintf(file, "%s", "[REQUEST_HANDLING_STARTED]: ");
    *seek += fprintf(file, "Начало обработки заявки. Идентификатор заявки - %d, оператор - %s\n", msg->ident_app, msg->name_op);
    break;

    case REQUEST_HANDLING_FINISHED:
    *seek += fprintf(file, "%s", "[REQUEST_HANDLING_FINISHED]: ");
    *seek += fprintf(file, "Закончена обработка заявки. Идентификатор %d, оператор %s, время выполнения - %d мин\n", msg->ident_app, msg->name_op, msg->time_treatment);
    break;

    case DEPARTMENT_OVERLOADED:
    *seek += fprintf(file, "%s", "[DEPARTMENT_OVERLOADED] ");
    *seek += fprintf(file, "Отделение %s перегружено. Перегрузка произошла после заявки с идентифкатором %d. ", msg->ident_otd, msg->ident_app);
    if (msg->name_op)
    {
        *seek += fprintf(file, "Отделение %s передало свои заявки отделению %s\n", msg->ident_otd, msg->name_op);
    }
    else *seek += fprintf(file, "Отделение %s не может передать заявки никакому отделению\n", msg->ident_otd);
    break;

    default: break;
    }
    free(msg);
}
enum answer read_app(FILE* file, otd* otds, void* storage, enum heap eHeap, enum storage eStorage, 
                        int* seek, double coeff, enum answer (*insertHeap)(void*, application* app), void (*meld_heaps)(void*, void*), otd** least_load, char* ident_ll,
                         FILE* file_log, my_time* tm)
{
    //printf("ok\n");
    application* app = (application*)malloc(sizeof(application));
    if (!app)
    {
        return ERROR_MEMORY;
    }
    //[2024-03-06T11:04:41]
    if (tm->day == -1) 
    {
        fscanf(file, "%d-%d-%dT%d:%d:%d", &tm->year, &tm->mounth, &tm->day, &tm->hour, &tm->min, &tm->second);
    }
    else fscanf(file, "%s", app->time_app);
    fscanf(file, "%d %s",  &(app->prior), app->ident);
    app->ident[3] = '\0';
    app->text = malloc(sizeof(char) * 256);
    if (!app->text)
    {
        return ERROR_MEMORY;
    }
    fgets(app->text, 256, file);
    app->ident_app = *seek; //идентифкатор заявки - ее номер по счету
    //(*count_apps)++; //это общий счетчик заявок, нужен только для идентифкатора заявки
    void* stor;
    otd* otdd;
    if (eStorage == ARRAY)
    {
        DepartmentArray* stor = (DepartmentArray*)storage;
        otdd = search_department_array(storage, app->ident);
    }
    else if (eStorage == BST)
    {
        node_BST* stor = (node_BST*)storage;
        otdd = search_BST(stor, app->ident);
    }
    else if (eStorage == HASHSET)
    {
        hashtable* stor = (hashtable*)storage;
        otdd = searchMacro(stor, app->ident);
    }
    else if (eStorage == TRIE)
    {
        trie_node* stor = (trie_node*)storage;
        otdd = search_trie(stor, app->ident);
    }

        void* heap = otdd->heap;
        if (!heap)
        {
            return INVALID;
        }
        if (insertHeap(heap, app) != ERROR_MEMORY)
        {
            //printf("Поступление заявки, идентификатор отделения - %s, время поступления: %s, идентификатор заявки: %d\n", app->ident, app->time_app, app->ident_app);
            log_msg *msg = malloc(sizeof(log_msg));
            if (!msg)
                return ERROR_MEMORY;
            msg->eCode = NEW_REQUEST;
            msg->ident_otd = app->ident;
            msg->ident_app = app->ident_app;
            log_(file_log, msg, seek, tm);
        }
        else return ERROR_MEMORY;
        otdd->count_real_apps++;
        otdd->count_free_apps++;
        if (otdd->count_op != 0) otdd->coeff_load = (double)otdd->count_real_apps / (double)otdd->count_op; //сразу считаем(пересчитываем) коеф загруженности
        if (!(*least_load)) //если еще нет отделения с самой низкой загруженностью - делаем его
        {
            *least_load = otdd;
            strcpy(ident_ll, app->ident);
        }
        else
        { //иначе сравниваем кефы и если что меняем указатель
            if (otdd->coeff_load < (*least_load)->coeff_load) 
            {
                *least_load = otdd;
                strcpy(ident_ll, app->ident);
            }
        }
        if (otdd->coeff_load >= coeff) 
        {
            otdd->isOverload = 1;
            //printf("Отделение %s перегружено. Перегрузка произошла после заявки с идентифкатором %d\n", app->ident, app->ident_app);
            log_msg *msg = malloc(sizeof(log_msg));
            if (!msg)
                return ERROR_MEMORY;
            msg->eCode = DEPARTMENT_OVERLOADED;
            msg->ident_otd = app->ident;
            msg->ident_app = app->ident_app;
            if (*least_load && *least_load != otdd)
            {
                meld_heaps((*least_load)->heap, otdd->heap);
                msg->name_op = &(ident_ll[0]);
                otdd->coeff_load = 0; 
                (*least_load)->count_free_apps += otdd->count_free_apps;
                (*least_load)->count_real_apps += otdd->count_real_apps;
                otdd->count_free_apps = 0;
                otdd->count_real_apps = 0;
                (*least_load)->coeff_load = (*least_load)->count_real_apps / (*least_load)->count_op; 
                //передать заявления 
                //пересчитать кэф
            }
            else msg->name_op = NULL;
            log_(file_log, msg, seek, tm);
        }
        return OK;
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
enum answer process_apps(void* stor, enum heap eHeap, otd* otds, int count_otd, int max_time_app, double overload_coeff, FILE* file_log, int* seek, my_time* tm, my_time* end)
{
    application* (*extract_max)(void*);
    switch (eHeap)
    {
        case BINARY:
        extract_max = &extract_max_binary_heap;
        break;

        case BINOMIAL:
        extract_max = &extract_max_binomial;
        break;

        case FIBONACCI:
        extract_max = &extract_max_fib;
        break;

        case SKEW:
        extract_max = &extract_max_skew;
        break;

        case LEFTIST:
        extract_max = &extract_max_leftist;
        break;
    }
    int iter = 0;
    int success = 0;
    while (success != count_otd)
    {
        int i = 0;
        for (i; i < count_otd; i++)
        {
            if (does_time_finish(tm, end))
            {
                for (int k = i; k < count_otd; k++)
                {
                    for (int op = 0; op < otds[k].count_op; op++)
                    {
                        if (otds[k].operators[op].app) free_application(otds[k].operators[op].app);
                    }
                }
                return INVALID;
            }
            if (otds[i].isOverload != -1)
            {
                for (int op = 0; op < otds[i].count_op; op++)
                {
                    //printf("отделение %d  оператор номер %d\n", i, op);
                    if (otds[i].operators[op].app && otds[i].operators[op].time_process <= iter - otds[i].operators[op].start_time)
                    { // если оператор был занят и закончил
                        log_msg* msg = malloc(sizeof(log_msg));
                        if (!msg) return ERROR_MEMORY;
                        msg->eCode = REQUEST_HANDLING_FINISHED;
                        msg->ident_app =  otds[i].operators[op].app->ident_app;
                        msg->name_op = &(otds[i].operators[op].name[0]);
                        msg->time_treatment = otds[i].operators[op].time_process;
                        log_(file_log, msg, seek, tm);
                        // printf("Закончена обработка заявки. Идентификатор %d, оператор %s, время выполнения - %d мин\n",
                        //        otds[i].operators[op].app->ident_app, otds[i].operators[op].name, otds[i].operators[op].time_process);
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
                            // printf("Начало обработки заявки. Идентификатор заявки - %d, оператор - %s, приоритет : %d\n",
                            //      app->ident_app, otds[i].operators[op].name, app->prior);
                            log_msg *msg = malloc(sizeof(log_msg));
                            if (!msg)
                                return ERROR_MEMORY;
                            msg->eCode = REQUEST_HANDLING_STARTED;
                            msg->ident_app = otds[i].operators[op].app->ident_app;
                            msg->name_op = &(otds[i].operators[op].name[0]);
                            log_(file_log, msg, seek, tm);
                        }
                        else if (otds[i].count_real_apps == 0)
                        {
                            if (otds[i].isOverload != -1) 
                            {
                                success += 1;
                                otds[i].isOverload = -1;
                            }
                        }
                    }

                }
            }
        }
        iter += 1;
        add_time(tm);
    }
    
}
void free_storage(enum storage eStor, void* storage, void (*free_heap)(void*, int))
{
    switch(eStor)
    {
        case ARRAY:
        free_department_array(storage, free_heap);
        break;
        case BST:
        free_BST(storage, free_heap);
        break;
        case HASHSET:
        free_table(storage, free_heap);
        break;
        case TRIE:
        free_trie(storage, free_heap);
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
            return ERROR_MEMORY;
           }
        }
}
void* do_storage(FILE* file, otd* otds, int count, enum storage eStorage) //создаю выбранное хранилище и вставляю туда пары идентификатор(считывая с файла) - отделение(объект кучи)
{
    if (eStorage == ARRAY)
    {
        DepartmentArray* da = create_department_array(count);
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 4);
            if (!ident_key) return NULL;
            fscanf(file, "%s", ident_key);
            ident_key[4] = '\0';
            insert_department_array(da, ident_key, &(otds[i]));
            
        }
        return da;
    }
    else if (eStorage == BST)
    {
        node_BST* bst = NULL;
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 3);
            if (!ident_key) return NULL;
            fscanf(file, "%s", ident_key);
            if (!bst)  
            {
                bst = createnode_BST(ident_key, &(otds[i]));
            }
            else insert_BST(bst, ident_key, &(otds[i]));
        }
        return bst;
    }
    else if (eStorage == HASHSET)
    {
        hashtable* table = create_hash_table();
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 3);
            if (!ident_key) return NULL;
            fscanf(file, "%s", ident_key);
            insertMacro(table, ident_key, &(otds[i]));
        }
        return table;
    }
    else if (eStorage == TRIE)
    {
        trie_node* triee = create_node_trie();
        for (int i = 0; i < count; i++)
        {
            char* ident_key = malloc(sizeof(char) * 3);
            if (!ident_key) return NULL;
            fscanf(file, "%s", ident_key);
            insert_trie(triee, ident_key, &(otds[i]));
        }
        return triee;
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
    FILE* file_log = fopen("log.txt", "w");
    enum heap eHeap;
    enum storage eStorage;
    char start[19];
    my_time* end = (my_time*)malloc(sizeof(my_time));
    if (!end)
    {
        fclose(file1);
        printf("Ошибка в выделении памяти\n");
        return 0;
    }
    int min_time, max_time, count;
    double overload_coeff;
    otd* otds;
    read_from_file(file1, &eHeap, &eStorage);
    fscanf(file1, "%s", start);
    fscanf(file1, "%d-%d-%dT%d:%d:%d", &end->year, &end->mounth, &end->day, &end->hour, &end->min, &end->second);
    fscanf(file1, "%d %d %d", &min_time, &max_time, &count);
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
        do_heaps(otds, count, eHeap, min_time, max_time,  (void* (*) (void)) &create_binary_heap);
        foo_insert = &insert_binary_heap;
        free_heap = &free_binary_heap;
        meld = &mergeBinaryHeapsWithoutDestruction;
        break;
        case BINOMIAL:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &createBinomaialHeap);
        foo_insert = &insertBinomialHeap;
        free_heap = &freeHeapBinomial;
        meld = &mergeHeaps_without_destr;
        break;
        case FIBONACCI:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &make_fib_heap);
        foo_insert = &fib_heap_insert;
        free_heap = &free_fib_heap;
        meld = &fib_heap_merge_without_destr;
        break;
        case TREAP:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &create_treap_node);
        foo_insert = &insert_treap;
        free_heap = &free_treap;
        meld = &merge_treap_without_destr;
        break;
        case SKEW:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &create_skew_root);
        foo_insert = &insert_skew;
        meld = &merge_skew;
        free_heap = &free_skew;
        break;
        case LEFTIST:
        do_heaps(otds, count, eHeap, min_time, max_time, (void* (*) (void)) &create_leftist_root);
        foo_insert = &insert_leftist;
        meld = &merge_leftist;
        free_heap = &free_left;
        break;

    }
    void* stor = do_storage(file1, otds, count, eStorage);
    fscanf(file1, "%lf", &overload_coeff);
    fclose(file1);
    int i = 3, seek = 0;
    FILE* file;
    otd* least_load = NULL;
    char* ident_ll = (char*)malloc(sizeof(char) * 4);
    if (!ident_ll) 
    {
        printf("Ошибка выделения памяти\n");
            fclose(file);
            free(end);
            free_storage(eStorage, stor, free_heap);
            free(otds);
            return 0;
    }
    //ident_ll = NULL;
    my_time* curr = (my_time*)malloc(sizeof(my_time));
    if (!curr)
    {
        printf("Ошибка выделения памяти\n");
        fclose(file);
        free(curr);
        free(end);
        free_storage(eStorage, stor, free_heap);
        free(otds);
        return 0;
    }
    curr->day = -1;
    while(file = fopen(argv[i], "r"))
    {
        if (read_app(file, otds, stor, eHeap, eStorage, &seek, overload_coeff, foo_insert, meld, &least_load, ident_ll, file_log, curr) != OK)
        {
            printf("Ошибка выделения памяти\n");
            fclose(file);
            free(curr);
            free(end);
            free_storage(eStorage, stor, free_heap);
            free(otds);
            return 0;
        }
        i++;
        fclose(file);
    }
    process_apps(stor, eHeap, otds, count, max_time, overload_coeff, file_log, &seek, curr, end);
    free_storage(eStorage, stor, free_heap);
    fclose(file_log);
    free(curr);
    free(end);
    free(otds);
    free(ident_ll);
    return 0;
}