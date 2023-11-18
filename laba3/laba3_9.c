
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
enum checker
{
    OK,
    ERROR,
    INVALID
};
typedef struct Node 
{
    char* word;
    int count;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char* word)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->count = 1;
    new_node->word = word;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}
Node* createNode_forTree(char* word, int count)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->count = count;
    new_node->word = (char*)malloc(sizeof(char) * strlen(word));
    if (!new_node->word)
    {
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->word, word);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void saveTreeToFile(Node* root, FILE* file, char separator, int level) {
    for (int i = 0; i < level; i++)
    {
        fprintf(file, " ");
    }
    if (root == NULL) {
        fprintf(file, "NULL%c0\n", separator);
    } else {
        fprintf(file, "%s%c%d\n", root->word, separator, root->count);
        saveTreeToFile(root->left, file, separator, level + 3);
        saveTreeToFile(root->right, file, separator, level + 3);
    }
}
Node* load_tree_from_file(FILE* file, char separator)
{
    char word[100];
    int count;
    int i = 0;
    char c = fgetc(file);
    while (isspace(c))
    {
        c = fgetc(file);
    }
    if (c == separator)
    {
        word[0] = ' ';
        word[1] = '\0';
    }
    while (c != separator)
    {
        word[i] = c;
        i++;
        c = fgetc(file);
    }
    word[i] = '\0';
    fscanf(file, "%d\n", &count);
    if (strcmp(word, "NULL") == 0)
    {
        return NULL;
    }
    Node* newNode = createNode_forTree(word, count); 
    newNode->left = load_tree_from_file(file, separator);
    newNode->right = load_tree_from_file(file, separator);
    return newNode;
    
}
void destroy_tree(Node* current)
{
    if (current == NULL) return;
    free(current->word);
    destroy_tree(current->left);
    destroy_tree(current->right);
    free(current);
}
void insert(Node** root, char* word)
{
    if (*root == NULL)
    {
        *root = createNode(word);
    }
    else
    {
        Node* current = *root;
        int flag = 0;
        while(!flag)
        {
            if (strcmp(word, current->word) == 0)
            {
                (current->count)++;
                free(word);
                flag = 1;
            }
            else if (strcmp(word, current->word) < 0)
            {
                if (current->left == NULL)
                {
                    current->left = createNode(word);
                    flag = 1;
                }
                else
                {
                    current = current->left;
                }
            }
            else
            {
                if (current->right == NULL)
                {
                    current->right = createNode(word);
                    flag = 1;
                }
                else
                {
                    current = current->right;
                }
            }
        }
    }
}
int check_s_in_mas(char c, char* mas)
{
    int len = strlen(mas);
    for (int i = 0; i < len; i++)
    {
        if (mas[i] == c) return 1;
    }
    return 0;
}
enum checker my_scanf(FILE* in, char* mas_separators, int* index, char* str, char** word)
{
    fseek(in, SEEK_SET, *index);
    char c = fgetc(in);
    int size = 10;
    *word = (char *)malloc(sizeof(char) * size);
    int i = *index;
    int index_word = 0;
    while (!check_s_in_mas(c, mas_separators) && c != EOF)
    {
        if (index_word >= size)
        {
            char* temp = realloc(*word, sizeof(char) * size * 2);
            if (!temp)
            {
                free(*word);
                return ERROR;
            }
            size *= 2;
            *word = temp;
        }
        (*word)[index_word] = c;
        index_word++;
        i++;
        c = fgetc(in);
    }
    (*word)[index_word] = '\0';
    if (c == EOF)
    {
       *index = -1;
       return OK;
    }
    *index = i + 1;
    return OK;
}
int find_count_in_file(Node* root, char* word)
{
    Node* current = root;
    while(current)
    {
        if (strcmp(word, current->word) == 0)
        {
            return current->count;
        }
        else if (strcmp(word, current->word) < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return 0;
}
void foo(Node* current, int n, int* max, char** mas)
{
    if (current == NULL) return;
    int m = 0;
    int ok = 0;
    while (m < n && !ok)
    {
        if (current->count > max[m])
        {
            max[m] = current->count;
            mas[m] = current->word;
            ok = 1;
        }
        m++;
    }
    foo(current->left, n, max, mas);
    foo(current->right, n, max, mas);
}
enum checker first_n(Node* root, int n, char** mas)
{
    Node* current = root;
    int* max = (int*)malloc(sizeof(int) * n);
    if (!max)
    {
        return ERROR;
    }
    for (int i = 0; i < n; i++)
    {
        max[i] = 0;
        mas[i] = '_';
    }
    foo(current, n, max, mas);
    free(max);
}

void max_min_len(Node* current, int* max, int* min, char** max_len, char** min_len)
{
    if (current == NULL) return;
    if (strlen(current->word) > *max)
    {
        *max = strlen(current->word);
        if (*max_len != NULL)
        {
            free(*max_len);
        }
        *max_len = (char*)malloc(sizeof(char) * (*max) + 1);
        if (!(*max_len))
        {
                if (*min_len) free(*min_len);
                *max_len = NULL;
                *max = -1;
                return;
         }
        strcpy(*max_len, current->word);
    }
    else if (strlen(current->word) < *min)
    {
        *min = strlen(current->word);
        if (*min_len != NULL)
        {
            free(*min_len);
        }
        *min_len = (char*)malloc(sizeof(char) * (*min) + 1);
        if (!(*min_len))
        {
            free(*max_len);
            *min_len = NULL;
            *min = -1;
            return;
        }
        strcpy(*min_len, current->word);
    }
    max_min_len(current->left, max, min, max_len, min_len);
    max_min_len(current->right, max, min, max_len, min_len);
}
int maxDepth(Node* root) {
    if (root == NULL) {
        return 0;
    } else {
        int leftDepth = maxDepth(root->left);
        int rightDepth = maxDepth(root->right);

        if (leftDepth > rightDepth) {
            return leftDepth + 1;
        } else {
            return rightDepth + 1;
        }
    }
}
void print_node(Node* node)
{
    if (node == NULL) return;
    printf("Слово: %s, встречалось %d раз\n", node->word, node->count);
}
void print_all(Node* root)
{
    if (root == NULL) return;
    print_node(root);
    print_all(root->left);
    print_all(root->right);
}

int main(int argc, char*argv[])
{
    if (argc < 3)
    {
        printf("Не подан текстовый файл и разделители\n");
        return 1;
    }
    FILE* in;
    if (!(in = fopen(argv[1], "r")))
    {
        printf("Не удалось открыть входной файл\n");
        return 1;
    }
    char* separator = malloc(sizeof(char) * (argc - 2));
    if (!separator)
    {
        printf("Ошибка памятм\n");
        return 0;
    }
    for (int i = 2; i < argc; i++)
    {
        if (strlen(argv[i]) > 3)
        {
            free(separator);
            fclose(in);
            printf("Длина одного из переданных аргументов > 1 => не символ\n");
        }
        separator[i - 2] = argv[i][0];
    } 
    Node* root = NULL;
    char line[1000];
    int count_words = 0;
    while (fgets(line, sizeof(line), in))
    {
        int index = 0;
        while (index < 1000 && index != -1 && line[index] != EOF)
        {
            char* word;
            my_scanf(in, separator, &index, line, &word);
            insert(&root, word);
            count_words++;
        }
    }
    char my_sep = separator[0];
    free(separator);

    char user_str[2];
    while(strcmp(user_str, "exit") != 0)
    {
        int max_d = maxDepth(root);
        printf("Максимальная глубина построенного дерева: %d\n", max_d);
        printf("1 - для вывода каждого слова и его количества, 2 - для поиска количества введенного слова, 3 - для вывода n наиболее часто встречающихся слов, \
        4 - для вывода слов с максимальной и минимальной длиной, 5 - для вывода содержимого дерева в текстовый файл и 6 для чтения в новое дерево из файла\n");
        scanf("%s", user_str);
        if (strcmp(user_str, "1") == 0)
        {
            print_all(root);
        }
        if (strcmp(user_str, "2") == 0)
        {
           char word[50];
           printf("Введите слово:\n");
           scanf("%s", word);
           printf("Это слово встретилось %d раз\n", find_count_in_file(root, word)); 
        }
        if (strcmp(user_str, "3") == 0)
        {
            int n;
            printf("Введите N: \n");
            scanf("%d", &n);
            printf("%d\n", count_words);
            if (n > count_words)
            {
                printf("Введено слишком большое число\n");
            }
            else
            {
                char **mmas = malloc(sizeof(char *) * n);
                if (!mmas)
                {
                    printf("Ошибка памятм\n");
                    ///
                    return 0;
                }
                first_n(root, n, mmas);
                for (int i = 0; i < n; i++)
                {
                    printf("%d %s\n", i + 1, mmas[i]);
                }
                free(mmas);
            }  
        }
        if (strcmp(user_str, "4") == 0)
        {
            char* max_len = NULL;
            char* min_len = NULL;
            int max = 0;
            int min = INT_MAX;
            max_min_len(root, &max, &min, &max_len, &min_len);
            int i = 0;
            printf("max: %s\n", max_len);
            printf("min: %s\n", min_len);
            free(max_len);
            free(min_len);
        }
        if (strcmp(user_str, "5") == 0)
        {
            char file_[15];
            printf("Введите название текстового файла куда занести дерево\n");
            scanf("%s", file_);
            FILE* for_save = fopen(file_, "w");
            if (!for_save)
            {
                printf("Не удалось создать файл\n");
            }
            else
            {

                saveTreeToFile(root, for_save, my_sep, 0);
                printf("Готово\n");
                fclose(for_save);
            }
        }
        if (strcmp(user_str, "6") == 0)
        {
            char file_[15];
            printf("Введите название текстового файла откуда прочитать дерево\n");
            scanf("%s", file_);
            FILE* for_read = fopen(file_, "r");
            if (!for_read)
            {
                printf("Не удалось открыть файл\n");
            }
            else
            {
                Node* root_save = load_tree_from_file(for_read, my_sep);
                printf("Вот что удалось прочитать:\n");
                if (root_save == NULL)
                {
                    printf("Файл пуст\n");
                }
                else
                {
                    print_all(root_save);
                    destroy_tree(root_save);
                }
                fclose(for_read);
            }
        }
    }
    destroy_tree(root);
    fclose(in);
    return 0;
}