#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Node *findNextPreOrder(Tree *tree, Node *x);
int findNextForLastPreOrder(Tree *tree);
int findNextForPrevPreOrder(Tree *tree, Node *x);
Node *findPrevInOrder(Node *x);

Node **preOrderTraversal(Node *ptr, Node **ptr_list, int *list_len)
{
    if (ptr == NULL)
        return ptr_list;
    ptr_list = realloc(ptr_list, (*list_len + 1) * sizeof (Node *));
    ptr_list[(*list_len)++] = ptr;
    ptr_list = preOrderTraversal(ptr->left, ptr_list, list_len);
    ptr_list = preOrderTraversal(ptr->right, ptr_list, list_len);
    return ptr_list;
}

Node **inOrderTraversal(Node *ptr, Node **ptr_list, int *list_len)
{
    if (ptr == NULL)
        return ptr_list;

    ptr_list = inOrderTraversal(ptr->left, ptr_list, list_len);
    ptr_list = realloc(ptr_list, (*list_len + 1) * sizeof (Node *));
    ptr_list[(*list_len)++] = ptr;
    ptr_list = inOrderTraversal(ptr->right, ptr_list, list_len);
    return ptr_list;
}

//void addThreadsInOrder(Tree *tree)
//{
//    Node **ptr_list = NULL;
//    int list_len = 0;
//    ptr_list = inOrderTraversal(tree->root, ptr_list, &list_len);
//    for (int i = 0; i < list_len - 1; i++)
//        ptr_list[i]->next = ptr_list[i + 1];
//    free(ptr_list);
//}

Node *makeNode(const key_t key, const char *info)
{
    if (info == NULL)
        return NULL;
    Node *node = malloc(sizeof (Node));
    node->key = key;
    node->info = malloc(strlen(info) + 1);
    strcpy(node->info, info);
    node->left = NULL; node->right = NULL; node->parent = NULL; node->next = NULL;
    return node;
}

Node *findParentNode(const Tree *tree, const Node *to_insert)
{
    Node *ptr = tree->root;
    Node *parent = tree->root;
    while (ptr != NULL)
    {
        parent = ptr;
        if (to_insert->key < ptr->key)
            ptr = ptr->left;
        else if (to_insert->key > ptr->key)
            ptr = ptr->right;
        else if (to_insert->key == ptr->key)
        {
            if (ptr->right == NULL || ptr->right->key > to_insert->key)
                return ptr;
            else
                ptr = ptr->right;
        }
    }
    return parent;
}

int attach(Node *to_insert, Node *parent, Tree *tree)
{
    if (to_insert == NULL || parent == NULL)
        return -1;
    //printf("attach\n");
    if (to_insert->key < parent->key)
    {
        to_insert->parent = parent;
        parent->left = to_insert;
    }
    if (to_insert->key > parent->key)
    {
        to_insert->parent = parent;
        parent->right = to_insert;
    }
    if (to_insert->key == parent->key)
    {
        to_insert->parent = parent;
        to_insert->right = parent->right;
        if (parent->right != NULL)
            parent->right->parent = to_insert;
        parent->right = to_insert;
    }
    to_insert->next = findNextInOrder(to_insert);
    Node *prev = findPrevInOrder(to_insert);
    if (prev != NULL)
        prev->next = to_insert;
    //printf("next1\n");
    //Node *next = findNextPreOrder(tree, to_insert);
    //printf("next2\n");
    //findNextForLastPreOrder(tree);
    //findNextForPrevPreOrder(tree, next);
    return 0;
}

int insert(Tree *tree, Node *to_insert)
{
    if (tree == NULL)
        return -1;
    //printf("%d ", to_insert->key);
    if (tree->root == NULL)
    {
        tree->root = to_insert;
        return 0;
    }
    attach(to_insert, findParentNode(tree, to_insert), tree);
    //printf("Traversion: ");
    //for (Node *ptr = tree->root; ptr != NULL; ptr = ptr->next)
        //printf("%d ", ptr->key);
//    printf("Pre-order traversal: ");
//    for (int i = 0; i < list_len; i++)
//    {
//        printf("%d ", ptr_list[i]->key);
//    }
    //printf("\n");
    return 0;
}

int delPostOrder(Node **pptr)
{
    if (*pptr == NULL)
        return 0;
    delPostOrder(&(*pptr)->left);
    delPostOrder(&(*pptr)->right);
    free((*pptr)->info);
    free(*pptr);
    return 0;
}

int deleteTree(Tree *tree)
{
    if (tree == NULL)
        return -1;
    delPostOrder(&tree->root);
    tree->root = NULL;
    return 0;
}

Node *findFirst(Node *root, const key_t key)
{
    Node *ptr = root;
    while (ptr != NULL)
    {
        if (ptr->key == key)
            return ptr;
        else if (key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }
    return NULL;
}

Node *findFirstMax(Node *root)
{
    Node *ptr = root;
    if (ptr == NULL)
        return NULL;
    while (ptr->right != NULL)
        ptr = ptr->right;
    return ptr;
}

Node *findFirstMin(Node *root)
{
    Node *ptr = root;
    if (ptr == NULL)
        return NULL;
    while (ptr->left != NULL)
        ptr = ptr->left;
    return ptr;
}

Node *findNextInOrder(Node *x)
{
    if (x->right != NULL)
        return findFirstMin(x->right);
    Node *ptr = x->parent;
    while (ptr != NULL && x == ptr->right)
    {
        x = ptr;
        ptr = x->parent;
    }
    return ptr;
}

Node *findPrevInOrder(Node *x)
{
    if (x->left != NULL)
        return findFirstMax(x->left);
    Node *ptr = x->parent;
    while (ptr != NULL && x == ptr->left)
    {
        x = ptr;
        ptr = x->parent;
    }
    return ptr;
}

Node *findNextPreOrder(Tree *tree, Node *x)
{
    Node *ptr = x;
    if (ptr == tree->root && tree->root->next == NULL)
        return NULL;
    if (ptr->next != NULL && (ptr->next == ptr->left || ptr->next == ptr->right))
        return ptr->next;
    if (x != tree->root && x == x->parent->left)
    {
        //printf("(%d is left)\n", x->key);
        while (ptr != tree->root && (ptr == x || (ptr->right == NULL || findFirst(ptr->right, x->key))))
        {
            //printf("\nptr%d->right: %p, parent %d\n", ptr->key, ptr->right, ptr->parent->key);
            ptr = ptr->parent;
            //printf("lol\n");
        }
    }
    else if (x != tree->root && x == x->parent->right)
        while (ptr != tree->root && (ptr == x || (ptr->right == NULL || findFirst(ptr->right, x->key))))
        {
            //printf("\nptr%d->right: %p\n", ptr->key, ptr->right);
            ptr = ptr->parent;
        }
    //printf("returned\n");
    if (ptr == tree->root)
    {
        if (findFirst(tree->root->right, x->key) == NULL)
            return ptr->right;
        else
            return NULL;
    }
    return ptr->right;
}

int findNextForPrevPreOrder(Tree *tree, Node *x)
{
    Node *ptr = tree->root;
    while (ptr->next != x)
        ptr = ptr->next;
    //printf("ptr_prev = %d\n", ptr->key);
    ptr->next = findNextPreOrder(tree, ptr);
    //if (ptr->next != NULL)
        //printf("ptr_next_for_prev%d = %d\n", ptr->key, ptr->next->key);
    return 0;
}

int findNextForLastPreOrder(Tree *tree)
{
    Node *ptr = tree->root;
    while (ptr->next != NULL)
    {
        //printf("ptr_key %d", ptr->key);
        ptr = ptr->next;
    }
    //printf("ptr_key %d", ptr->key);
    if (ptr == tree->root)
        printf("root\n");
    ptr->next = findNextPreOrder(tree, ptr);
    //printf("\nptr%d->next: %p\n", ptr->key, ptr->next);
    return 0;
}

int delete(Tree *tree, Node *to_delete)
{
    if (tree == NULL || to_delete == NULL)
        return -1;
    Node *victim = NULL;
    if (to_delete->left == NULL || to_delete->right == NULL)
        victim = to_delete;
    else
        victim = findNextInOrder(to_delete);
    //if (victim)
    {
        //Node *next = findNextInOrder(to_delete);
        //printf("to_delete %d\n", victim->key);
        Node *prev = findPrevInOrder(victim);
        //printf("prev %d\n", prev->key);
        if (prev != NULL)
            prev->next = findNextInOrder(victim);
        //printf("prev->next %d\n", prev->next->key);
    }
    Node *subtree = NULL;
    if (victim->left != NULL)
        subtree = victim->left;
    else
        subtree = victim->right;
    Node *parent = victim->parent;
    if (subtree != NULL)
        subtree->parent = parent;
    if (parent == NULL)
        tree->root = subtree;
    else
    {
        if (parent->left == victim)
            parent->left = subtree;
        else
            parent->right = subtree;
    }
    if (victim != to_delete)
    {
        to_delete->key = victim->key;
        to_delete->info = realloc(to_delete->info, strlen(victim->info) + 1);
        strcpy(to_delete->info, victim->info);
    }

    free(victim->info);
    free(victim);
    getchar();
    getchar();
    return 0;
}

Node **find(Node *root, const key_t key)
{
    if (root == NULL)
        return NULL;
    Node **array = NULL;
    int array_len = 0;
    Node *ptr = root;
    while (ptr != NULL)
    {
        if (ptr->key == key)
        {
            array = realloc(array, (array_len + 1) * sizeof (Node *));
            array[array_len++] = ptr;
            ptr = ptr->right;
        }
        else if (key < ptr->key)
            ptr = ptr->left;
        else
            ptr = ptr->right;
    }
    array = realloc(array, (array_len + 1) * sizeof (Node *));
    array[array_len] = NULL;
    return array;
}

Node **findMax(Node *root)
{
    if (root == NULL)
        return NULL;
    Node *ptr = root;
    while (ptr->right != NULL)
        ptr = ptr->right;
    return find(root, ptr->key);
}
