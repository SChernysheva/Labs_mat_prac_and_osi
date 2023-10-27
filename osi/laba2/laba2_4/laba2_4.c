#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
enum checkFiles
{
    OK,
    ERROR_FILE,
    ERROR_PROCCESS,
    OVERFLOW
};
int checkTxt(char *str)
{
    return (strstr(str, ".txt") == str + strlen(str) - 4) ? 1 : 0;
}
int checkStr(FILE *in, char *str)
{
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        return -2;
    }

    int *ans = (int *)shmat(shmid, NULL, 0);
    if ((int)ans == -1)
    {
        return -2;
    }
    *ans = 0;
    pid_t pid = fork();
    if (pid < 0)
    {
        return -2;
    }
    else if (pid == 0)
    {
        char c = fgetc(in);
        int len = strlen(str);
        while (c != EOF)
        {
            if (str[0] == c)
            {
                if (len == 1)
                {
                    *ans = 1;
                }
                c = fgetc(in);
                if (c == EOF)
                {
                    _Exit(0);
                }
                int i = 1;
                while (c == str[i] && str[i])
                {
                    c = fgetc(in);
                    i++;
                    if (c == EOF)
                    {
                        if (i == len)
                        {
                            *ans = 1;
                        }
                        _Exit(0);
                    }
                }
                if (i == len)
                {
                    *ans = 1;
                    _Exit(0);
                }
            }
            c = fgetc(in);
        }
        _Exit(0);
    }
    else
    {
        wait(NULL);
        int res = *ans;
        shmdt(ans);
        shmctl(shmid, IPC_RMID, NULL);
        return res;
    }
}
enum checkFiles get_name_of_file(char names[256], FILE *in, char *str)
{
    char c;
    int namesIndex = 0;
    while ((c = fgetc(in)) != EOF) //while(!feof(in))
    {
        if (!isspace(c))
        {
            char name[20];
        int i = 0;
        if (!isspace(c))
        {
            while (c != EOF && c != ' ' && c != '\n' && c != '\t')
            {
                name[i] = c;
                i++;
                c = fgetc(in);
            }
        } 
        name[i] = '\0';
        int check_txt = checkTxt(name);
        if (check_txt == 0)
        {
            return ERROR_FILE;
        }
        FILE *file;
        if (!(file = fopen(name, "r")))
        {
            return ERROR_FILE;
        }
        int ans = checkStr(file, str);
        if (ans == 1)
        {
            if (namesIndex + i + 2 > 256)
            {
                fclose(file);
                return OVERFLOW;
            }
            for (int m = 0; m < i; m++)
            {
                names[namesIndex] = name[m];
                namesIndex++;
            }
            names[namesIndex] = ' ';
            namesIndex++;
        }
        if (ans == -2)
        {
            fclose(file);
            return ERROR_PROCCESS;
        }
        fclose(file);
        }
    }
    names[namesIndex] = '\0';
    return OK;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Передайте путь к файлу и строку для поиска\n");
        return 0;
    }
    FILE *in;
    if ((checkTxt(argv[1]) == 0) || !(in = fopen(argv[1], "r")))
    {
        printf("Не удалось открыть файл\n");
        return 0;
    }
    char names[256];

    enum checkFiles ans = get_name_of_file(names, in, argv[2]);
    if (ans == OK)
    {
        int i = 0;
        if (!(names[i])){
            printf("Ни в одном файле нет заданной строки\n");
            fclose(in);
            return 0;
        }
        while (names[i])
        {
            printf("%c", names[i]);
            i++;
        }
        printf("\n");
        fclose(in);
        return 0;
    }
    else if (ans == OVERFLOW)
    {
        printf("Передано слишком большое кол-во файлов или их имена слишком длинные\n");
        fclose(in);
        return 0;
    }
    else if (ans == ERROR_FILE)
    {
        printf("Не удалось открыть один из переданных файлов или формат не тхт\n");
        fclose(in);
        return 0;
    }
    else
    {
        printf("Ошибка в процессе / разделяемой памяти\n");
        fclose(in);
        return 0;
    }
    return 0;
}