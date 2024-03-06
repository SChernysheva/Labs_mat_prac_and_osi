#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"

typedef struct SkewHeap
{
    application* app;
    struct SkewHeap* right;
    struct SkewHeap* left;
}skew_heap;
typedef struct SkewHeapRoot
{
    skew_heap* root;
}skew_root;


skew_heap* create_skew_node(application* app)
{
    skew_heap* newNode = (skew_heap*)malloc(sizeof(skew_heap));
    if (!newNode) return NULL;
    newNode->app = app;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}
skew_root* create_skew_root()
{
    skew_root* new = (skew_root*)malloc(sizeof(skew_root));
    if (!new) return NULL;
    new->root = NULL;
    return new;
}
skew_heap* merge(skew_heap* h1, skew_heap* h2) {
    if (h1 == NULL) {
        return h2;
    }
    if (h2 == NULL) {
        return h1;
    }
    
    if (  (h1->app->prior > h2->app->prior) 
    ||( (h1->app->prior == h2->app->prior)  &&  (strcmp(h1->app->time_app, h2->app->time_app) < 0)))
     {
        skew_heap* tmp = h1->left;
        h1->left = h1->right;
        h1->right = merge(tmp, h2);
        return h1;
    } else {
        skew_heap* tmp = h2->left;
        h2->left = h2->right;
        h2->right = merge(tmp, h1);
        return h2;
    }
}

void merge_skew(skew_root* sr1, skew_root* sr2) {
    sr1->root = merge(sr1->root, sr2->root);
    sr2->root = NULL; 
}

application* extract_max_skew(skew_root* sr) {
    if (sr == NULL || sr->root == NULL) {
        return NULL; // Куча пуста
    }
    
    // Сохраняем максимальное значение
    application* maxApp = sr->root->app;
    
    // Объединяем правое и левое поддерево корня
    skew_heap* oldRoot = sr->root;
    sr->root = merge(sr->root->left, sr->root->right);
    
    // Освобождаем память, занимаемую предыдущим корнем
    free(oldRoot);
    
    return maxApp;
}

skew_root* insert_skew(skew_root* sr, application* newApp) {
    skew_heap* newHeap = create_skew_node(newApp);
    newHeap->app = newApp;
    newHeap->left = NULL;
    newHeap->right = NULL;
    
    // Объединяем существующую кучу с новым элементом
    sr->root = merge(sr->root, newHeap);
    
    return sr;
}

void free_skew(skew_heap* root, int isFull)
{
     if (root == NULL)
        return;

    free_skew(root->left, isFull);
    free_skew(root->right, isFull);    
    free(root);
}