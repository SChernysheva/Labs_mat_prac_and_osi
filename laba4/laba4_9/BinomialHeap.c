#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"
//нихуя не работает все переделать
//время посутпления


typedef struct Node {
    application* data;
    int degree;
    struct Node* child;
    struct Node* sibling;
    struct Node* parent;
} node;

typedef struct BinomialHeap {
    node* head;
} binomial_heap;


node* createNode(application* data) {
    node* new_node = (node*)malloc(sizeof(node));
    if (!new_node) return NULL;
    new_node->data = data;
    new_node->degree = 0;
    new_node->child = NULL;
    new_node->sibling = NULL;
    new_node->parent = NULL;
    return new_node;
}

binomial_heap* createBinomaialHeap() {
    binomial_heap* bh = (binomial_heap*)malloc(sizeof(binomial_heap));
    if (!bh) return NULL;
    bh->head = NULL;
    return bh;
}
void freeNode(node* node, int isFull) {
    if (node == NULL) {
        return;
    }

    freeNode(node->child, isFull);

    freeNode(node->sibling, isFull);
    if(isFull) free(node->data);
    free(node);
}

void freeHeapBinomial(binomial_heap* heap, int isFull) {
    if (heap == NULL) {
        return;
    }

    freeNode(heap->head, isFull);

    heap->head = NULL;

    free(heap);
}
node* mergeTrees(node* tree1, node* tree2) {
    if (!tree1) {
        return tree2;
    }
    if (!tree2) {
        return tree1;
    }
    if (tree1->data->prior < tree2->data->prior) {
        node* temp = tree1;
        tree1 = tree2;
        tree2 = temp;
    }
    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}
node* mergeHeaps_without_destr_in(node* heap1, node* heap2) {
    if (!heap1) return heap2;
    if (!heap2) return heap1;

    node* mergedHeap = NULL;
    node** lastPtr = &mergedHeap;
    node* temp1 = heap1;
    node* temp2 = heap2;

    while (temp1 && temp2) {
        node** nextPtr = temp1->degree <= temp2->degree ? &temp1 : &temp2; // Указатель на кучу с меньшей степенью

        *lastPtr = *nextPtr;
        lastPtr = &((*lastPtr)->sibling);
        *nextPtr = (*nextPtr)->sibling;
    }

    *lastPtr = temp1 ? temp1 : temp2;

    node* prev = NULL;
    node* curr = mergedHeap;
    node* next;

    while (curr && curr->sibling) {
        next = curr->sibling;

        if (next->degree != curr->degree || (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if ( (curr->data->prior > next->data->prior) ||
                (curr->data->prior == next->data->prior && strcmp(curr->data->time_app, next->data->time_app) < 0)) {
                curr->sibling = next->sibling;
                mergeTrees(curr, next);
            } else {
                if (prev) {
                    prev->sibling = next;
                } else {
                    mergedHeap = next;
                }
                mergeTrees(next, curr);
                curr = next;
            }
        }
    }

    return mergedHeap;
}

void mergeHeaps_without_destr(binomial_heap* heap1, binomial_heap* heap2) 
{
    heap1->head = mergeHeaps_without_destr_in(heap1->head, heap2->head);
    heap2->head = NULL;
}

// application* extractMaxBinomial(binomial_heap* H) {
//     int max = 0;
//     node* x = NULL;
//     node* xBefore = NULL;
//     node* curx = H->head;
//     node* curxBefore = NULL;

//     while (curx) {
//         if (curx->data->prior > max) {
//             max = curx->data->prior;
//             x = curx;
//             xBefore = curxBefore;
//         }
//         curxBefore = curx;
//         curx = curx->sibling;
//     }

//     if (!x) {
//         printf("max node not found\n");
//         return NULL;
//     }

//     if (!xBefore) {
//         H->head = x->sibling;
//     } else {
//         xBefore->sibling = x->sibling;
//     }

//     binomial_heap* Hdash = createBinomaialHeap();
//     Hdash->head = x->child;

//     curx = Hdash->head;
//     while (curx) {
//         curx->parent = NULL;
//         curx = curx->sibling;
//     }

//     mergeHeaps_without_destr(H, Hdash);
//     free(Hdash);
//     application* app = x->data;
//     free(x);
//     return app;
// }
application* extract_max_binomial(binomial_heap* H) {
    if (H == NULL || H->head == NULL) {
        printf("Heap is empty\n");
        return NULL;
    }

    int max = H->head->data->prior;
    node* maxNode = H->head;
    node* prevMaxNode = NULL;
    node* prev = NULL;
    node* current = H->head;

    while (current->sibling) {
        if (current->sibling->data->prior > max ) {
            max = current->sibling->data->prior;
            maxNode = current->sibling;
            prevMaxNode = current;
        }
        prev = current;
        current = current->sibling;
    }

    // Удаляем найденный максимальный узел из кучи
    if (prevMaxNode) {
        prevMaxNode->sibling = maxNode->sibling;
    } else {
        H->head = maxNode->sibling;
    }

    binomial_heap* newHeap = createBinomaialHeap();
    newHeap->head = maxNode->child;

    current = newHeap->head;
    while (current) {
        current->parent = NULL;
        current = current->sibling;
    }

    // Освобождаем узел, возвращаем данные и очищаем память
    application* extractedData = maxNode->data;
    free(maxNode);
    mergeHeaps_without_destr(H, newHeap);
    free(newHeap);

    return extractedData;
}

enum answer insertBinomialHeap(binomial_heap* bh, application* data) {
    node* newNode = createNode(data);
    bh->head = mergeHeaps_without_destr_in(bh->head, newNode); // Выполняем слияние куч

    return OK;
}
binomial_heap* merge_binomial_destr(binomial_heap* bh1, binomial_heap* bh2)
{
    binomial_heap* new_bh = createBinomaialHeap();
    if (!new_bh)
    {

    }
    while(bh1->head)
    {
        application* app = extract_max_binomial(bh1);
        insertBinomialHeap(new_bh, app);
    }
    while(bh2->head)
    {
        application* app = extract_max_binomial(bh2);
        insertBinomialHeap(new_bh, app);
    }
    freeHeapBinomial(bh1, 0);
    freeHeapBinomial(bh2, 0);
    return new_bh;
}


