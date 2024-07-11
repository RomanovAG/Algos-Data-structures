#ifndef BTREE_H
#define BTREE_H

#define MAX_KEY_NUM 3
#define MAX_INFO_NUM MAX_KEY_NUM

#define LAST_KEY_INDEX 2
#define LAST_INFO_INDEX LAST_KEY_INDEX

typedef struct Node
{
    int key_num;
    char *key[MAX_KEY_NUM];
    unsigned *info[MAX_INFO_NUM];
    struct Node *next[MAX_KEY_NUM + 1];
} Node;

typedef struct B_Tree
{
    Node *root;
} B_Tree;

typedef struct Key_pos
{
    Node *ptr;
    int index;
} Key_pos;

Node *makeEmptyNode();
Node **preOrderTraversal(Node *ptr, Node **ptr_list, int *list_len);
void timingInOrderTraversal(Node *ptr, int *list_len);
Key_pos **inOrderTraversal(Node *ptr, Key_pos **kp_list, int *list_len);
int bDeleteTree(Node **root);
Key_pos *bSearch(Node *root, const char *key);
Key_pos *bMoreSearch(Node *root, const char *key);
int bInsert(Node **root, const char *key, const unsigned info);
int bDelete(Node **root, const char *key);
Key_pos *findMax(Node *root);
Key_pos *findMin(Node *root);
int printTree(const Node *ptr, int space);

#endif // BTREE_H
