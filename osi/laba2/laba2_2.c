#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

int main() {
    // int pid;
    // pid = fork();
    // printf("%d\n", pid);
    fork();
    printf("Hi\n"); 
    fork(); 
    printf("Hi\n");
    return 0;
}