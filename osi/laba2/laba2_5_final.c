#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

sem_t women_mutex; 
sem_t men_mutex;
sem_t counter_mutex;
int counter = 0; // Счетчик людей в ванной комнате    если <0 женщины(их кол-во равно модулю)  если >0 мужчины
int max_count;
void woman_wants_to_enter()
{
    int success = 0;
    sem_wait(&women_mutex); 
    sem_wait(&counter_mutex);
    printf("Женщина хочет зайти\n");
    if (counter <= 0 && abs(counter) + 1 <= max_count)
    {
        counter = counter - 1;
        printf("Женщина вошла в ванную комнату\n");
        sem_post(&counter_mutex);
        sem_post(&women_mutex);
        success = 1;
    }
    else
    {
        if (counter > 0) printf("Женщина ожидает, в ванной мужчины\n");
        else printf("Ванная переполнена, женщина ожидает\n");
        sem_post(&counter_mutex);
        sem_post(&women_mutex);
        while (!success)
        {
            sem_wait(&women_mutex);
            sem_wait(&counter_mutex);
            if (counter <= 0 && abs(counter) + 1 <= max_count)
            {
                counter = counter - 1;
                printf("Женщина вошла в ванную комнату\n");
                sem_post(&counter_mutex);
                sem_post(&women_mutex);
                success = 1;
            }
            else
            {
                sem_post(&counter_mutex);
                sem_post(&women_mutex);
            }
        }
    }
}

void man_wants_to_enter()
{
    int success = 0;
    sem_wait(&men_mutex);
    sem_wait(&counter_mutex);
    printf("Мужчина хочет зайти\n");
    if (counter >= 0 && counter + 1 <= max_count)
    {
        counter = counter + 1;
        printf("Мужчина вошел в ванную комнату\n");
        success = 1;
        sem_post(&counter_mutex);
        sem_post(&men_mutex);
    }
    else
    {
        if (counter < 0) printf("Ванная занята женщинами, мужчина ждет\n");
        else printf("Ванная переполнена, мужчина ожидает\n");
        sem_post(&counter_mutex);
        sem_post(&men_mutex);
        while (!success)
        {
            sem_wait(&men_mutex);
            sem_wait(&counter_mutex);

            if (counter >= 0)
            {
                counter = counter + 1;
                printf("Мужчина вошел в ванную комнату\n");
                success = 1;
                sem_post(&counter_mutex);
                sem_post(&men_mutex);
            }
            else
            {
                sem_post(&counter_mutex);
                sem_post(&men_mutex);
            }
        }
    }
}

void woman_leaves()
{
    sem_wait(&counter_mutex);
    counter = counter + 1;
    printf("Женщина покинула ванную комнату\n");
    sem_post(&counter_mutex);
}

void man_leaves()
{
    sem_wait(&counter_mutex);
    counter = counter - 1;
    sem_post(&counter_mutex);
    printf("Мужчина покинул ванную комнату\n");
}
void *woman(void *arg)
{
    woman_wants_to_enter();
    sleep(rand() % 3);
    woman_leaves();
    return NULL;
}

void *man(void *arg)
{
    man_wants_to_enter();
    sleep(rand() % 3);
    man_leaves();
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Необходимо указать параметр N\n");
        return 1;
    }

    int N = atoi(argv[1]);
    max_count = N;
    srand(time(NULL));

    sem_init(&women_mutex, 0, 1);
    sem_init(&men_mutex, 0, 1);
    sem_init(&counter_mutex, 0, 1);

    pthread_t women[N];
    pthread_t men[N];

    for (int i = 0; i < N * 2; i++)
    {
        pthread_create(&women[i], NULL, woman, NULL);
        pthread_create(&men[i], NULL, man, NULL);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(women[i], NULL);
        pthread_join(men[i], NULL);
    }

    sem_destroy(&women_mutex);
    sem_destroy(&men_mutex);
    sem_destroy(&counter_mutex);

    return 0;
}