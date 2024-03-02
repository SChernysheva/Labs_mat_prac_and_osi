#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"

DepartmentArray* createDepartmentArray(int initialCapacity) {
    DepartmentArray* da = (DepartmentArray*)malloc(sizeof(DepartmentArray));
    //TODO!!
    da->capacity = initialCapacity;
    da->size = 0;
    da->keys = (char**)malloc(initialCapacity * sizeof(char*));
    //TODO!!!
    da->values = (otd**)malloc(initialCapacity * sizeof(otd*));
    //TODO!!!!
    da->least_load = NULL;
    return da;
}

void insertDepartmentArray(DepartmentArray* da, char* key, otd* value) {
    if (da->size == da->capacity) {
        da->capacity *= 2;
        da->keys = (char**)realloc(da->keys, da->capacity * sizeof(char*));
        //TODO!!
        da->values = (otd**)realloc(da->values, da->capacity * sizeof(otd*));
        //TODO!!
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

// Function to search for a key in the department array
otd* searchDepartmentArray(DepartmentArray* da, char* key) {
    for (int i = 0; i < da->size; i++) {
        if (strcmp(key, da->keys[i]) == 0) {
            return da->values[i];
        }
    }
    return NULL;
}
void freeDepartmentArray(DepartmentArray* da, void(*free_heap)(void*, int)) {
    for (int i = 0; i < da->size; i++) {
        free(da->keys[i]);
        free_heap(da->values[i]->heap, 1);
        free(da->values[i]->operators);
    }
    free(da->keys);
    free(da->values);
    free(da);
}

