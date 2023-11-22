#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
//#define HASHSIZE 128

enum checker
{
    OK,
    ERROR_MEMORY,
    INVALID
};
enum tabl
{
    _OK,
    NOT_OK
};

typedef struct Macros {
    char* key;
    char* value;
    unsigned long hash;
    struct Macros* next;
} macros;

typedef struct HashTable {
    macros** items;
    int size;
} hashtable;

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash;
}
enum tabl check(hashtable* hash_table, int* count)
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (int i = 0; i < hash_table->size; i++)
    {
        int m = 0;
        macros* cur = hash_table->items[i];
        while (cur != NULL)
        {
            m++;
            (*count)++;
            cur = cur->next;
        }
        if (m > max) max = m;
        if (m < min) min = m;
    }
    if (min == 0) 
    {
        if (max >= 2) return NOT_OK;
        else return _OK; 
    }
    if (max / min >= 2) 
    {
        return NOT_OK;
    }
    return _OK;
}
enum checker create_hash_table(hashtable **table, int size) {
	*table = (hashtable*)malloc(sizeof(hashtable));

	if (!*table) {
		return ERROR_MEMORY;
	}

	(*table)->size = size;
	(*table)->items = (macros**)malloc(sizeof(macros*) * (*table)->size);

	if (!(*table)->items) {
		free((*table)->items);
		(*table)->items = NULL;
		return ERROR_MEMORY;
	}

	for (int i = 0; i < (*table)->size; i++) {
		(*table)->items[i] = NULL;
	}

	return OK;
}
enum checker insertMacro(hashtable* hash_table, char *key, char *value) {

    unsigned long h_value = hash(key);
    unsigned long h = h_value % hash_table->size;
    macros* newMacro = (macros*)malloc(sizeof(macros));
    if (!newMacro)
    {
        return ERROR_MEMORY;
    }
    newMacro->hash = h_value;
    newMacro->key = strdup(key);
    newMacro->value = strdup(value);
    newMacro->next = hash_table->items[h];
    hash_table->items[h] = newMacro;
    return OK;
}

char* searchMacro(hashtable *hash_table, char *key) {
    unsigned long h_value = hash(key);
    unsigned long h = h_value % hash_table->size;
    macros *mac = hash_table->items[h];
    while(mac != NULL) {
        if(strcmp(mac->key, key) == 0) {
            return mac->value;
        }
        mac = mac->next;
    }
    return NULL;
}

enum checker read_define(FILE* file, hashtable* hash_table)
{
    char line[100];
    char define[8];
    char def_name[100];
    char value[100];
    int fl = 0;
    fscanf(file, "%s", define);
    while (strcmp(define, "#define") == 0)
    {
        fscanf(file, "%s %s\n", def_name, value);
        //printf("%s %s\n", def_name, value);
        if (insertMacro(hash_table, def_name, value) != OK) return ERROR_MEMORY;
        fscanf(file, "%s", define);
    }
    fseek(file, ftell(file) - strlen(define), SEEK_SET);
    return OK;
}


enum checker do_define_v3(hashtable* hash_table, FILE* file)
{
    int start_index = ftell(file);
    char str[256];
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file) - start_index + 1;
    fseek(file, start_index, SEEK_SET);

    char *buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        return ERROR_MEMORY;
    }
    int index_buf = 0;
    char c = fgetc(file);
    int EO = 0;
    while(c != EOF && !isalnum(c))
    {
        if (index_buf == fileSize)
        {
            char* temp = realloc(buffer, fileSize * 2);
            if (!temp)
            {
                return ERROR_MEMORY;
            }    
            buffer = temp;
            fileSize *= 2;
        }
        buffer[index_buf] = c;
        index_buf++;
        c = fgetc(file);
    }
    if (c == EOF) EO = 1;
    fseek(file, -1, SEEK_CUR);
    while (!EO && fscanf(file, "%s", str) == 1)
    {
        if (index_buf == fileSize)
        {
            char* temp = realloc(buffer, fileSize * 2);
            if (!temp)
            {
                return ERROR_MEMORY;
            }    
            buffer = temp;
            fileSize *= 2;
        }
        char *value = searchMacro(hash_table, str);
        if (value != NULL)
        {
            if (index_buf + strlen(value) >= fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return ERROR_MEMORY;
                }
                buffer = temp;
                fileSize *= 2;
            }
            for (int j = 0; j < strlen(value); j++)
            {
                buffer[index_buf] = value[j];
                index_buf++;
            }
        }
        else
        {
            if (index_buf + strlen(str) >= fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return ERROR_MEMORY;
                }
                buffer = temp;
                fileSize *= 2;
            }
            index_buf += sprintf(&(buffer[index_buf]), "%s", str);
        }
        c = fgetc(file);
        while (c != EOF && !isalnum(c))
        {
            if (index_buf == fileSize)
            {
                char *temp = realloc(buffer, fileSize * 2);
                if (!temp)
                {
                    return ERROR_MEMORY;
                }
                buffer = temp;
                fileSize *= 2;
            }
            buffer[index_buf] = c;
            index_buf++;
            c = fgetc(file);
        }
        if (c == EOF) EO = 1;
        fseek(file, -1, SEEK_CUR);
    }
    printf("%s\n", buffer);
    fseek(file, start_index, SEEK_SET);
    for (int i = 0; i < index_buf; i++)
    {
        fprintf(file, "%c", buffer[i]);
    }
    while( ( c = fgetc(file)) != EOF )
    {
        fprintf(file, "%c", ' ');
    } 
    free(buffer);
    return OK; 
}
void print_h_t(hashtable* table)
{
    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i] != NULL)
        {
            macros* cur = table->items[i];
            while (cur != NULL)
            {
                printf("%s ", cur->value);
                cur = cur->next;
            }
            printf("\n");
        }
    }
}
void free_table(hashtable* my_table)
{
    for (int i = 0; i < my_table->size; i++)
    {
        if (my_table->items[i] != NULL)
        {
            macros* current = my_table->items[i];
            while(current != NULL)
            {
                macros* prev = current;
                current = current->next;
                free(prev->key);
                free(prev->value);
                free(prev);
            }
            free(current);
        }
    }
    free(my_table->items);
    free(my_table);
}

enum checker change_size_of_table(hashtable** table, int new_size)
{
    hashtable* new_table;
    if (create_hash_table(&new_table, new_size) != OK) return ERROR_MEMORY;
    for (int i = 0; i < (*table)->size; i++)
    {
        if ((*table)->items[i] != NULL)
        {
            macros* cur = (*table)->items[i];
            while (cur != NULL)
            {
                unsigned long hash = cur->hash;
                int new_index = hash % new_size;

                macros *newMacro = (macros *)malloc(sizeof(macros));
                if (!newMacro)
                {
                    return ERROR_MEMORY;
                }
                newMacro->hash = hash;
                newMacro->key = strdup(cur->key);
                newMacro->value = strdup(cur->value);
                newMacro->next = new_table->items[new_index];
                new_table->items[new_index] = newMacro;
                cur = cur->next;
            }
        }
    }
    free_table(*table);
    *table = new_table;
    return OK;
}

int main(int argc, char*argv[])
{
    if (argc < 2)
    {
        printf("Не подан текстовый файл\n");
        return 1;
    }
    FILE* in;
    if (!(in = fopen(argv[1], "r+")))
    {
        printf("Не удалось открыть входной файл\n");
        return 1;
    }
    int current_size = 128;
    hashtable* my_table;
    create_hash_table(&my_table, current_size);
    macros** HashTable = my_table->items;
    if (read_define(in, my_table) != OK)
    {
        printf("Ошибка памяти\n");
        free_table(my_table);
        return 1;
     }
    int waiting_count = 0;
    print_h_t(my_table);
    if (check(my_table, &waiting_count) != _OK)
    {
        int new_size = (int)(waiting_count * 2);
        printf("%d\n", new_size);
        if (change_size_of_table(&my_table, new_size) != OK)
        {

        }

    }
    print_h_t(my_table);
    if (do_define_v3(my_table, in) != OK)
    {
        printf("Ошибка\n");
    }
    fclose(in);
    free_table(my_table);
    return 0;
}