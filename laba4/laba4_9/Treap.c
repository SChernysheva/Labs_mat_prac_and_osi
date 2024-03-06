#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"

typedef struct Node_treap {
    application* app;
    int prior;
    struct Node_treap* left;
    struct Node_treap* right;
} treap_node;

treap_node* create_treap_node(application* app) {
    treap_node* newNode = (treap_node*)malloc(sizeof(treap_node));
    if (!newNode) return NULL;
    newNode->app = app;
    newNode->prior = app->prior;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void split(treap_node* root, application* app, treap_node** left, treap_node** right) {
    if (root == NULL) {
        *left = NULL;
        *right = NULL;
    } else if ((root->app->prior < app->prior)
    || ((root->app->prior < app->prior) && (strcmp(root->app->time_app, app->time_app) > 0))) {
        *left = root;
        split(root->right, app, &root->right, right);
    } else {
        *right = root;
        split(root->left, app, left, &root->left);
    }
}

treap_node* merge_treap_without_destr(treap_node* left, treap_node* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;

    if (left->prior > right->prior) {
        left->right = merge_treap_without_destr(left->right, right);
        return left;
    } else {
        right->left = merge_treap_without_destr(left, right->left);
        return right;
    }
}

void insert_treap(treap_node* root, application* app) {
    treap_node* newNode = create_treap_node(app);
    treap_node* left, *right;
    split(root, app, &left, &right);
    root = merge_treap_without_destr(merge_treap_without_destr(left, newNode), right);
}


treap_node* erase(treap_node *root, int key) {
    if (root == NULL) {
        return NULL;
    }
    if (root->app->ident_app == key) {
        treap_node* temp = root;
        root = merge_treap_without_destr(root->left, root->right);
        free(temp);
    } else if (root->app->ident_app < key) {
        root->right = erase(root->right, key);
    } else {
        root->left = erase(root->left, key);
    }
    return root;
}

treap_node* extract_max_treap(treap_node** root) {
    treap_node* current = *root;
    while (current->right) {
        current = current->right;
    }
    *root = erase(*root, current->app->ident_app);
    return current;
}
void free_treap(treap_node* node)
{
    if (!node) return;
    if (node) {
        free_treap(node->left);
        free_treap(node->right);
    }
    free(node);
}