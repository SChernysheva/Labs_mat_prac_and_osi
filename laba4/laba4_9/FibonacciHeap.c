#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <math.h>
#include "structures.h"

typedef struct Fib_heap_node {
    application* app;
    int degree;
    struct Fib_heap_node* parent;
    struct Fib_heap_node* child;
    struct Fib_heap_node* left;
    struct Fib_heap_node* right;
    int mark;
}fib_heap_node;

typedef struct Fib_Heap {
    fib_heap_node* max;
    int num_nodes;
}fib_heap;

fib_heap* make_fib_heap() {
    fib_heap* heap = (fib_heap*)malloc(sizeof(fib_heap));
    if (!heap)
    {
        return NULL;
    }
    heap->max = NULL;
    heap->num_nodes = 0;
    return heap;
}

fib_heap_node* make_fib_node(application* app) {
    fib_heap_node* node = (fib_heap_node*)malloc(sizeof(fib_heap_node));
    if (!node)
    {
        return NULL;
    }
    node->app = app;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;
    return node;
}

enum answer fib_heap_insert(fib_heap* heap, application* app) {
    fib_heap_node* node = make_fib_node(app);
    if (!node)
    {
        return ERROR_MEMORY;
    }
    if (heap->max == NULL) {
        heap->max = node;
    } else {
        node->left = heap->max->left;
        node->right = heap->max;
        heap->max->left->right = node;
        heap->max->left = node;
        if ((node->app->prior > heap->max->app->prior) || 
        (node->app->prior == heap->max->app->prior && strcmp(node->app->time_app, heap->max->app->time_app) < 0)) {
            heap->max = node;
        }
    }
    heap->num_nodes++;
    return OK;
}
void free_node_fib_helper(fib_heap_node *node, int isFull) {
       if (node == NULL) {
        return;
    }
    fib_heap_node* child = node->child;
    if (child != NULL) {
        fib_heap_node* temp = child;
        do {
            fib_heap_node* next = child->right;
            free_node_fib_helper(child, isFull);
            child = next;
        } while (child != temp);
    }
    //if (node->app) free_application(node->app);
     //free(node);
}

void free_fib_heap(fib_heap* heap, int isFull) {
    if (heap == NULL) {
        return;
    }
    //free_node_fib_helper(heap->max, isFull);
    free(heap);
}
void fib_heap_merge_without_destr(fib_heap* H1, fib_heap* H2) {
    if (H1->max == NULL) {
        H1->max = H2->max;
    } else if (H2->max != NULL) {
        // сливаем списки корней
        fib_heap_node* temp = H1->max->right;
        H1->max->right = H2->max->right;
        H2->max->right->left = H1->max;
        H2->max->right = temp;
        temp->left = H2->max;

        if (H2->max->app->prior > H1->max->app->prior ||
            (H2->max->app->prior == H1->max->app->prior && strcmp(H2->max->app->time_app, H1->max->app->time_app) < 0)) {
            H1->max = H2->max;
        }
    }
    H1->num_nodes += H2->num_nodes;
    H2->num_nodes = 0;
}
fib_heap* merge_fib_heaps_destr(fib_heap* heap1, fib_heap* heap2) {
    fib_heap* new_heap = make_fib_heap();
    
    if (heap1->max == NULL) {
        new_heap->max = heap2->max;
    } else if (heap2->max != NULL) {
        // Merge the root lists of heap1 and heap2
        fib_heap_node* temp = heap1->max->right;
        heap1->max->right = heap2->max->right;
        heap2->max->right->left = heap1->max;
        heap2->max->right = temp;
        temp->left = heap2->max;
        
        // Update the max pointer of the new heap
        if (heap2->max->app->prior > heap1->max->app->prior ||
            (heap2->max->app->prior == heap1->max->app->prior && strcmp(heap2->max->app->time_app, heap1->max->app->time_app) < 0)) {
            new_heap->max = heap2->max;
        } else {
            new_heap->max = heap1->max;
        }
    } else {
        new_heap->max = heap1->max;
    }
    
    new_heap->num_nodes = heap1->num_nodes + heap2->num_nodes;
    
    // Free the memory of the input heaps
    // free_fib_heap(heap1);
    // free_fib_heap(heap2);
    free(heap1);
    free(heap2);
    heap1 = NULL;
    heap2 = NULL;
    
    return new_heap;
}
void unionLists(fib_heap_node* first, fib_heap_node* second) {
    if (first == NULL || second == NULL) {
        return;
    }

    fib_heap_node* firstNext = first->right;
    fib_heap_node* secondPrev = second->left;

    first->right = second;
    second->left = first;
    firstNext->left = secondPrev;
    secondPrev->right = firstNext;
}

void consolidate(fib_heap* heap) {
    if (heap == NULL || heap->max == NULL) {
        return;
    }
    int MAXDEGREE = log2(heap->num_nodes);
    fib_heap_node** A = (fib_heap_node**)malloc(sizeof(fib_heap_node*) * MAXDEGREE); // Массив для хранения узлов по их степеням
    //todo
    for (int i = 0; i < MAXDEGREE; i++) {
        A[i] = NULL;
    }
    fib_heap_node* max = heap->max;
    fib_heap_node* current = heap->max->right;
    while (current != heap->max) {
        int degree = current->degree;
        fib_heap_node* next = current->right;
        while (A[degree] != NULL) {
            fib_heap_node* conflict = A[degree];
            fib_heap_node* addTo;
            fib_heap_node* adding;
            if (conflict->app->prior > current->app->prior) {
                addTo = conflict;
                adding = current;
            } else {
                addTo = current;
                adding = conflict;
            }
            unionLists(addTo->child, adding);
            adding->parent = addTo;
            addTo->degree++;

            A[degree] = NULL;
            degree++;
        }
        A[degree] = current;
        current = next;
    }

    for (int i = 0; i < MAXDEGREE; i++) {
        if (A[i] != NULL && A[i]->app->prior > max->app->prior  ) {
            max = A[i];
        }
    }

    heap->max = max;
    free(A);
}
application* find_max_fib(fib_heap* heap)
{
    return heap->max->app;
}

application* extract_max_fib(fib_heap* heap) {
    if (heap == NULL || heap->max == NULL) {
        return NULL; // Куча пуста, возвращаем NULL
    }

    fib_heap_node* maxNode = heap->max;
    application* maxApp = maxNode->app;
    // Удаление максимального узла из корневого списка
    if (maxNode->right == maxNode) {
        heap->max = NULL; // Был всего один узел, делаем кучу пустой
    } else {
        // Удаляем максимальный узел из корневого списка
        maxNode->left->right = maxNode->right;
        maxNode->right->left = maxNode->left;

        heap->max = maxNode->right; // Обновляем указатель на максимальный узел
    }

    // Объединяем списки дочерних узлов максимального узла с корневым списком
    if (maxNode->child != NULL) {
        fib_heap_node* child = maxNode->child;
        fib_heap_node* temp = child;
        do {
            fib_heap_node* next = child->right;
            child->parent = NULL; // Снимаем родительские связи у дочерних узлов
            unionLists(heap->max, child); // Вставляем дочерний узел в корневой список

            child = next;
        } while (child != temp);
    }
    free(maxNode);
    maxNode = NULL;
    // Проводим консолидацию для поддержания свойств кучи
    consolidate(heap);

    // Уменьшаем количество узлов в куче и возвращаем максимальный элемент
    heap->num_nodes--;
    return maxApp;
}
