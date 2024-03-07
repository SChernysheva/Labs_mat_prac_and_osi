#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"


typedef struct DepartmentArray {
    int capacity;
    int size;
    char** keys;
    otd** values; 
} DepartmentArray;

DepartmentArray* create_department_array(int initialCapacity) {
    DepartmentArray* da = (DepartmentArray*)malloc(sizeof(DepartmentArray));
    if (!da) return NULL;
    da->capacity = initialCapacity;
    da->size = 0;
    da->keys = (char**)malloc(initialCapacity * sizeof(char*));
    if (!da->keys)
    {
        free(da);
        return NULL;
    }
    da->values = (otd**)malloc(initialCapacity * sizeof(otd*));
    if (!da->values)
    {
        free(da->keys);
        free(da);
        return NULL;
    }
    //da->least_load = NULL;
    return da;
}

enum answer insert_department_array(DepartmentArray* da, char* key, otd* value) {
    if (da->size == da->capacity) {
        da->capacity *= 2;
        da->keys = (char**)realloc(da->keys, da->capacity * sizeof(char*));
        if (!da->keys)
        {
            free(da);
            return ERROR_MEMORY;
        }
        da->values = (otd**)realloc(da->values, da->capacity * sizeof(otd*));
        if (!da->values)
        {
            free(da->keys);
            free(da);
            return ERROR_MEMORY;
        }
    }
    
    int index = da->size;
    while (index > 0 && strcmp(key, da->keys[index - 1]) < 0) {
        da->keys[index] = da->keys[index - 1];
        da->values[index] = da->values[index - 1];
        index--;
    }
    
    da->keys[index] = key;
    da->values[index] = value;
    da->size++;
}

otd* search_department_array(DepartmentArray* da, char* key) {
    for (int i = 0; i < da->size; i++) {
        if (strcmp(key, da->keys[i]) == 0) {
            return da->values[i];
        }
    }
    return NULL;
}
void free_department_array(DepartmentArray* da, void(*free_heap)(void*, int)) {
    for (int i = 0; i < da->size; i++) {
        free(da->keys[i]);
        free_heap(da->values[i]->heap, 1);
        free(da->values[i]->operators);
    }
    free(da->keys);
    free(da->values);
    free(da);
}

