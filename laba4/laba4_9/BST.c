#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"


typedef struct node_BST {
    char* key;
    otd* otdd;
    struct node_BST *left;
    struct node_BST *right;
    otd* least_load;
} node_BST;

node_BST *createnode_BST(char* key, otd* otdd) {
    node_BST *newnode_BST = (node_BST*)malloc(sizeof(node_BST));
    if (!newnode_BST)
    {
        return NULL;
    }
    newnode_BST->key = key;
    newnode_BST->otdd = otdd;
    newnode_BST->left = NULL;
    newnode_BST->right = NULL;
    newnode_BST->least_load = NULL;
    return newnode_BST;
}
node_BST *insert_BST(node_BST *root, char* key, otd* value) {
    if (root == NULL) {
        return createnode_BST(key, value);
    }
    
    if (strcmp(key, root->key) < 0) {
        root->left = insert_BST(root->left, key, value);
    } else if (strcmp(key, root->key) > 0) {
        root->right = insert_BST(root->right, key, value);
    }
    
    return root;
}

otd *search_BST(node_BST *root, char* key) {
    if (root == NULL || strcmp(key, root->key) == 0) {
        return root->otdd;
    }
    if (strcmp(key, root->key) < 0) {
        return search_BST(root->left, key);
    } else {
        return search_BST(root->right, key);
    }
}

void free_BST(node_BST* root, void(*free_heap)(void*, int))
{
    if (root == NULL) {
        return;
    }
    free_BST(root->left, free_heap);
    free_BST(root->right, free_heap);
    if (root->key) free(root->key);
    free_heap(root->otdd->heap, 1);
    free(root->otdd->operators);
    // Освобождаем память для узла
    free(root);
}

