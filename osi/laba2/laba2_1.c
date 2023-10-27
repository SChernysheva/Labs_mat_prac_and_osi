#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

int main(){
    printf("Идентификатор текущего процесса: %d\n", getpid());
    printf("Идентификатор родительского процесса: %d\n", getppid());
    printf("Идентификатор группы вызывающего процесса: %d\n", getpgrp());
    printf("Реальный идентификатор владельца %d\n", getuid());
    printf("Реальный идентификатор группы владельца: %d\n", getgid());
    printf("Эффективный идентификатор владельца: %d\n", geteuid());
    printf("Эффективный идентификатор группы владельца: %d\n", getegid());

}