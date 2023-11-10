#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>
struct String {
    char* str;
    int length;
}typedef string;

struct Address{
    string town;
    string street;
    int num_of_house;
    string corpus;
    int num_of_flat;
    string index; //6
}typedef address;

struct Mail{
    address adrress_of_user;
    float weight;
    string identificator;//14
    string time_create;//19
    string time_give;//19
}typedef mail;

struct Post
{
    address* address_of_current_post;
    mail* mails;
    int count_mails;
}typedef post;

enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};
int isInt(char* str)
{
    while(*str)
    {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}
int isFloat(char* str)
{
    while(*str)
    {
        if (!isdigit(*str) && *str != '.') return 0;
        str++;
    }
    return 1;
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
    if (!my_str->str) return;
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

int comparator(const void* a, const void* b)
{
    const mail* mail1 = (const mail*)a;
    const mail* mail2 = (const mail*)b;
    if (mail1->adrress_of_user.index.str != mail2->adrress_of_user.index.str)
    {
        return strcmp(mail1->adrress_of_user.index.str, mail2->adrress_of_user.index.str);
    }
    else
    {
        return strcmp(mail1->identificator.str, mail2->identificator.str);
    }
}
void my_sort(mail* mails, int count, int (*comp)(const void*, const void*))
{
    qsort(mails, count, sizeof(mail), comp);
}
post init_post(address* adr)
{
    post post1;
    post1.address_of_current_post = adr;
    post1.mails = NULL;
    post1.count_mails = 0;
    return post1;
}
void print_post(post post1)
{
    for (int i = 0; i < post1.count_mails; i++)
    {
        printf("Посылка с индексом: %d\n", i + 1);
        printf("Город: %s\n", post1.mails[i].adrress_of_user.town.str);
        printf("Улица: %s\n", post1.mails[i].adrress_of_user.street.str);
        printf("Номер дома: %d\n", post1.mails[i].adrress_of_user.num_of_house);
        printf("Вес: %lf\n", post1.mails[i].weight);
        printf("Время создания: %s\n", post1.mails[i].time_create.str);
        printf("Время вручения: %s\n", post1.mails[i].time_give.str);
        printf("Почтовый индентификатор: %s\n", post1.mails[i].identificator.str);
        printf("---------------------\n");
    }
}
enum checker find_by_ident(char* ident, post post1, mail* mail1)
{
    for (int i = 0; i < post1.count_mails; i++)
    {
        if (strcmp(post1.mails[i].identificator.str, ident) == 0)
        {
            *mail1 = post1.mails[i];
            return OK;
        }
    }
    return INVALID;
}
enum checker add_mail_in_post(post* post1, mail mail1)
{
    if ((*post1).mails == NULL)
    {
        post1->mails = (mail*)malloc(sizeof(mail) * 1);
        if (!(*post1).mails) return ERROR_MEMORY;
        (*post1).mails[0] = mail1;
        (post1->count_mails)++;
        return OK;
    }
    mail* temp =(mail*)realloc(post1->mails, sizeof(mail) * ((post1->count_mails) + 1));
    if (!temp)
    {
        return ERROR_MEMORY;
    }
    post1->mails = temp;

    // if (!post1->mails) 
    // {
    //     free(post1->mails);
    //     return ERROR_MEMORY;
    // }
    post1->mails[post1->count_mails] = mail1;
    (post1->count_mails)++;
    my_sort(post1->mails, post1->count_mails, comparator);
    return OK;
}
void delete_one_mail(mail* mail1)
{
    delete_str(&(mail1->identificator));
    delete_str(&(mail1->time_create));
    if(mail1->time_give.length != -1) delete_str(&(mail1->time_give));
    if (mail1->adrress_of_user.corpus.length != -1) delete_str(&(mail1->adrress_of_user.corpus));
    delete_str(&(mail1->adrress_of_user.index));
    delete_str(&(mail1->adrress_of_user.street));
    delete_str(&(mail1->adrress_of_user.town));
}
enum checker remove_mail_from_post(post* post1, int index)
{
    //printf("%d\n", index);
    if (index < 0 || index > post1->count_mails || post1->count_mails == 0)
    {
        return INVALID;
    }
    delete_one_mail( &( post1->mails[index - 1] )); //1 2  0 1 
    (post1->count_mails)--;
    if (post1->count_mails == 0)
    {
        post1->mails = NULL;
        return OK;
    } 
    for (int i = index - 1; i < post1->count_mails - 1; i++)
    {
        post1->mails[i] = post1->mails[i + 1];
    }
    post1->mails = realloc(post1->mails, sizeof(mail) * (post1->count_mails));
    my_sort(post1->mails, post1->count_mails, comparator);
    return OK;
}
void get_time(char* timee)
{
    char res[20];
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(res, sizeof(res), "%d:%m:%Y %H:%M:%S", timeinfo);
    for (int i = 0; i <= 19; i++)
    {
        *timee = res[i];
        timee++;
    }
}
void free_post(post* post1)
{
    for (int i = 1; i <= post1->count_mails; i++)
    {
        remove_mail_from_post(post1, i);
    }
    free(post1->address_of_current_post->index.str);
    free(post1->address_of_current_post->street.str);
    free(post1->address_of_current_post->town.str);
}
int main()
{
    string town = make_string("Togliatty");
    string street = make_string("Leninskyi");
    string index = make_string("123456");
    int num_of_house = 12;
    address adr;
    adr.town = town;
    adr.street = street;
    adr.num_of_house = num_of_house;
    adr.index = index;
    post my_post = init_post(&adr);

    int flag;
    char c[1];

    printf("Введите цифру 1 если вы хотитe добавить mail в post, 2 если хотите его удалить, 3 для печати поста, 4 для поиска посылки по идентификатору, 5 для вывода недоставленных посылок или любую другую клавишу если хотите выйти из редактора\n");
    scanf("%s", c);
    if (!isdigit(c[0]) || strlen(c) > 1) flag = -1;
    else flag = toInt(c[0]);
    while (flag != -1)
    {
        my_sort(my_post.mails, my_post.count_mails, comparator);
        if (flag == 1)
            {
                mail my_mail;
                printf("Нужно собрать посылку\n");

                char town[25];
                char street[25];
                char num[10];
                printf("Введите через пробел город, улицу и номер дома получателя\n");
                scanf("%s %s %s", town, street, num);

                while (!isInt(num) || !street || !town || isInt(town) || isInt(street))
                {
                    printf("Такого адрес не существует, введите еще раз\n");
                    scanf("%s %s %s", town, street, num);
                }
                int atoi_num = atoi(num);
                my_mail.adrress_of_user.town = make_string(town);
                my_mail.adrress_of_user.street = make_string(street);
                my_mail.adrress_of_user.num_of_house = atoi_num;

                char id[15];
                printf("Введите идентификатор посылки (14 цифр)\n");
                scanf("%s", id);
                id[15] = '\0';
                while (strlen(id) < 14 || !isInt(id))
                {
                    printf("Идентификатор должен содержать 14 цифр, введите еще раз\n");
                    scanf("%s", id);
                }
                my_mail.identificator = make_string(id);

                char ind[6];
                printf("Введите индекс получателя (6 цифр)\n");
                scanf("%s", ind);
                while (strlen(ind) < 6 || !isInt(ind))
                {
                    printf("Индекс должен содержать 6 цифр, введите еще раз\n");
                    scanf("%s", ind);
                }
                my_mail.adrress_of_user.index = make_string(ind);

                char weight[10];
                printf("Введите вес посылки\n");
                scanf("%s", weight);
                while (!isFloat(weight))
                {
                    printf("Некорректный вес, введите еще раз\n");
                    scanf("%s", weight);
                }
                my_mail.weight = atof(weight);

                char time[20];
                get_time(time);
                my_mail.time_create = make_string(time);
                char sec[1];
                int day, hour, minute, year, mounth;
                //printf("%s\n", time);
                sscanf(time,"%d:%d:%d %d:%d:%s", &day, &mounth, &year, &hour, &minute, sec);
                int year_give = year;
                if (mounth + 1 > 12) year++;
                int mounth_give = (mounth % 12) + 1;
                char time_g[20];
                sprintf(time_g, "%d:%d:%d %d:%d:%s", day, mounth_give, year_give, hour, minute, sec);
                my_mail.time_give = make_string(time_g);

                printf("\n");

                my_mail.adrress_of_user.corpus.length = -1;
                my_mail.time_give.length = -1;
                if (add_mail_in_post(&my_post, my_mail) == OK)
                {
                    printf("Мы добавили вашу посылку!\n");
                }
                else
                {
                    printf("Ошибка в выделении памяти, повторрите попытку\n");
                }
            }
            else if (flag == 2)
            {
                print_post(my_post);
                printf("Содержимое поста на данный момент, введите индекс посылки, которую хотите удалить\n");
                int index;
                char c1[5];
                scanf("%s", c1);
                if (!isInt(c1)) index = -1;
                else index = atoi(c1);
                if (remove_mail_from_post(&my_post, index) == OK)
                {
                    printf("Посылка удалена\n");
                }
                else 
                {
                    printf("Подан некорректный индекс, такой посылки нет\n");
                }
            }
            else if (flag == 3)
            {
                print_post(my_post);
            }
            else if (flag == 4)
            {
                mail mail_find;
                char ident[15];
                printf("Введите идентификатор для поиска\n");
                scanf("%s", ident);
                if (!isInt(ident))
                {
                    printf("Такого идентификатора нет\n");
                }
                else
                {
                    if (find_by_ident(ident, my_post, &mail_find) == OK)
                    {
                        printf("Даннные о найденой посылке:\n");
                        printf("Адрес:\n");
                        printf("%s %s %d %d\n", mail_find.adrress_of_user.town.str, mail_find.adrress_of_user.street.str, mail_find.adrress_of_user.num_of_house, mail_find.adrress_of_user.num_of_flat);
                        printf("Индекс получателя: %s,вес посылки: %lf\n",mail_find.adrress_of_user.index.str,  mail_find.weight);
                        printf("Время создания посылки: %s\n", mail_find.time_create.str);
                        printf("Плановое время получения: %s\n", mail_find.time_give.str);
                    }
                    else
                    {
                        printf("Такой посылки нет\n");
                    }
                }
            }
            else if (flag == 5)
            {
                if (my_post.count_mails == 0)
                {
                    printf("В посте нет посылок\n");
                }
                else
                {
                    printf("Идентификаторы недоставленных посылок:\n");
                    char my_time[20];
                    char time_give[20];
                    get_time(my_time);
                    char sec[1];
                    int day, hour, minute, year, mounth;
                    int day_, hour_, minute_, year_, mounth_;
                    sscanf(my_time, "%d:%d:%d %d:%d:%s", &day, &mounth, &year, &hour, &minute, sec);
                    for (int i = 0; i < my_post.count_mails; i++)
                    {
                        sscanf(my_post.mails[i].time_give.str, "%d:%d:%d %d:%d:", &day_, &mounth_, &year_, &hour_, &minute_);
                        if (year < year_ || mounth < mounth_ || day < day_ || hour < hour_ || minute < minute_)
                        {
                            printf("%s\n", my_post.mails[i].identificator.str);
                        }
                    }
                }
            }

            printf("Введите цифру 1 если вы хотитe добавить mail в post, 2 если хотите его удалить, 3 для печати поста, 4 для поиска посылки по идентификатору, 5 для вывода недоставленных посылок или любую другую клавишу если хотите выйти из редактора\n");
            scanf("%s", c);
            if (!isdigit(c[0]) || strlen(c) > 1)
                flag = -1;
            else
                flag = toInt(c[0]);
        }
        free_post(&my_post);
            
        
}

