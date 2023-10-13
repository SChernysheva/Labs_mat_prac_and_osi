#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

char fooXOR8(FILE* in){
    char res = 0;
    char c;
    while ((c = fgetc(in)) != EOF){
        res ^= c;
    }
    return res;
}

int toInt(char* num) {
    int minus = 0;
    char* ptr = num;
    if (num[0] == '-') {
        minus = 1;
        ptr++; //!
    }
    while(*ptr == '0') {ptr++;}
    int base = 10;
    int res = 0;
    while(*ptr){
        res = res * base + (isdigit(*ptr) ? *ptr - '0' : *ptr - 'A' + 10);
        ptr++;
    }
    return (minus == 1) ? -res : res;
}
char xor32(FILE* in){ //то что закомментированно - для проверки, что если до заполнения 4х байтовой группы не хватает байт - он сам заполнит нулями
    unsigned int res = 0;
    unsigned int current[1];
    //fseek(in, 0, SEEK_END);
    //int countElse = ftell(in) % 4; //кол-во байтов которые остаются после собрания 4хбайтовых групп
    //printf("%d\n", countElse);
    fseek(in, 0, SEEK_SET);
    while(fread(current, sizeof(int), 1, in) == 1){
        res ^= current[0];
    }
    // if (countElse > 0){
    //     unsigned char bytes[4];
    //     fread(bytes, sizeof(char), 4, in);
    //     for (int i = countElse; i < 4; i++){
    //         bytes[i] = 0;
    //     }
    //     int I;
    //     I = bytes[0];
    //     I <<= 8;
    //     I |= bytes[1];
    //     I <<= 8;
    //     I |= bytes[2];
    //     I<<= 8;
    //     I |= bytes[3];
    //     res ^= I;
    // }

    return res;
}

int hex(FILE* in, unsigned int mask){
    unsigned int current[1] = {0};
    int count = 0;
    fseek(in, 0, SEEK_SET);
    while (fread(current, sizeof(int), 1, in) == 1){
        if ( (current[0] ^ mask) == 0) { count+=1; }
    }
    return count;    
}

int main(int argc, char*argv[]){
    if (argc < 3){
        printf("Введите название файла\n");
        return 1;
    }

    FILE* in;
    if (!(in = fopen(argv[1], "rb"))){
        printf("Не удалось открыть файл\n");
        return 0;
    }

    if (strcmp(argv[2], "xor8") == 0){
        char ans = fooXOR8(in);
        printf("%d\n", ans);
    }
    if (strcmp(argv[2], "xor32") == 0){
        char res = xor32(in);
        printf("%d\n", res);
    }
    if (strcmp(argv[2], "hex") == 0){
        int mask = toInt(argv[3]); //проверку на аргв 3 
        char res = hex(in, mask);
        //printf("%d\n", 5 ^ 5);
        printf("%d\n", res);
    }


    return 0;
}