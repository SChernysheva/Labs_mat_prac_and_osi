#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum checker
{
    OK,
    ERROR
};
typedef struct Node {
    char value;
    struct Node* brother;
    struct Node* child;
    struct Node* parent;
} Node;

typedef struct Tree
{
    Node* root;
} Tree;

void delete_tree(Node* node)
{
    if (!node) return;
    delete_tree(node->child);
    free(node);
}

enum checker insert_child(Node* node, char data)
{
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n)
    {
        return ERROR;
    }
    n->value = data;
    n->child = NULL;
    n->brother = NULL;
    n->parent = node;
    if (node->child == NULL) node->child = n;
    else
    {
        Node *temp = node->child;
        while (temp != NULL)
        {
            temp = temp->brother;
        }
        temp->brother = n;
    }
    return OK;
}
enum checker insert_brother(Node* node, char data)
{
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n)
    {
        return ERROR;
    }
    n->value = data;
    n->child = NULL;
    n->brother = NULL;
    n->parent = node->parent;
    if (node->brother == NULL) node->brother = n;
    else
    {
        Node *temp = node;
        while (temp != NULL)
        {
            temp = temp->brother;
        }
        temp->brother = n;
    }

    return OK;
}
enum checker buildTree(Tree* tree, char* expression) {
    int len = strlen(expression);
    char s;
    int index = 0;
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
    {
        return ERROR;
    }
    node->value = expression[index];
    node->brother = NULL;
    node->child = NULL;
    node->parent = NULL;
    tree->root = node;
    index++;
    enum checker ans;
    Node* now = tree->root;
    for (index; index < len; index++)
    {
        s = expression[index];
        if (s == '(')
        {
            index++;
            if (insert_child(now, expression[index]) != OK)
            {
                return ERROR;
            }
            now = now->child;
        }
        if (s == ')')
        {
            now = now->parent;
        }
        if (s == ',')
        {
            index++;
            if (insert_brother(now, expression[index]) != OK)
            {
                return ERROR;
            }
            now = now->brother;
        }
    }
    return OK;
}


void printTree(Node* root, int level, FILE* out) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        fprintf(out, "%s", "    ");  
    }
    fprintf(out, "%c\n", root->value); 

    printTree(root->child, level + 1, out); 
    printTree(root->brother, level, out);   
}

void processExpressions(FILE* in, FILE* out) {
    char line[100];
    while (fgets(line, sizeof(line), in) != NULL) {
        Tree tree;
        tree.root = NULL;
        buildTree(&tree, line);
        printTree(tree.root, 0, out);
        fprintf(out, "%s", "\n");
        delete_tree(tree.root);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Введите входной и выходной файлы\n");
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    FILE* outputFile = fopen(argv[2], "w");

    if (!inputFile || !outputFile) {
        printf("Failed to open file.\n");
        return 1;
    }

    processExpressions(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}