#ifndef MYLIB_H
#define MYLIB_H

typedef unsigned int key_t;

typedef struct Node
{
    key_t key;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    struct Node *left, *right, *parent;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    struct Node *next;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    char *info;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
} Node;

typedef struct Tree
{
    Node *root;
} Tree;

int insert(Tree *tree, Node *to_insert);
int deleteTree(Tree *tree);
int delete(Tree *tree, Node *to_delete);
Node *makeNode(const key_t key, const char *info);
Node *findFirst(Node *root, const key_t key);
Node *findFirstMin(Node *root);
Node *findFirstMax(Node *root);
Node *findNextInOrder(Node *x);
Node **find(Node *root, const key_t key);
Node **findMax(Node *root);

Node **preOrderTraversal(Node *ptr, Node **ptr_list, int *list_len);
//void addThreads(Tree *tree);

#endif // MYLIB_H
