#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>


#define COUNT_PHIL 4
#define KEY 1234
int sem_id;

void awayForks(int phil)
{
    int left = phil;
    int right = (phil + 1) % COUNT_PHIL;
    struct sembuf sem_lock_left = {left, +1, 0};
    struct sembuf sem_lock_right = {right, +1, 0};
    semop(sem_id, &sem_lock_left, 1);
    semop(sem_id, &sem_lock_right, 1);
}
void doPhil(int phil)
{
    int left = phil;
    int right = (phil + 1) % COUNT_PHIL;
    int success = 0;
    struct sembuf sem_lock_left = {left, -1, 0};
    struct sembuf sem_lock_right = {right, -1, IPC_NOWAIT};
    struct sembuf sem_unlock_left = {left, +1, 0};
    printf("Философ %d голоден\n", phil + 1);
    while(!success)
    {
        if (semop(sem_id, &sem_lock_left, 1) == 0)
        {
            if (semop(sem_id, &sem_lock_right, 1) == 0)
            {
                printf("Философ %d ест\n", phil + 1);
                sleep(rand() % 3);
                awayForks(phil);
                success = 1;
            }
            else 
            {
                semop(sem_id, &sem_unlock_left, 1);
                printf("Философ %d думает\n", phil + 1);
                sleep(rand() % 3);
            }
        }
        else{
            printf("Философ %d думает\n", phil + 1);
            sleep(rand() % 3); 
        }
    }
    printf("Философ %d думает\n", phil + 1);
}


int main()
{

    srand(time(NULL));
    pid_t philos[COUNT_PHIL];
    sem_id = semget(KEY, COUNT_PHIL, 0666 | IPC_CREAT);
    for (int i = 0; i < COUNT_PHIL; i++) // вилки
    {
        //struct sembuf sem_init = {i, +1, 1};
        semctl(sem_id, i, SETVAL, 1);
    }

    for (int i = 0; i < COUNT_PHIL; i++)
    {
        philos[i] = fork();
        if (philos[i] == -1)
        {
            perror("Error");
            return 1;
        }
        if (philos[i] == 0)
        {
            doPhil(i);
            return 0;
        }
    }
    for (int i = 0; i < COUNT_PHIL; i++)
    {
        waitpid(philos[i], NULL, 0);
    }
    semctl(sem_id, 0, IPC_RMID);
    return 0;
    
}
