#include "b-tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Node **preOrderTraversal(Node *ptr, Node **ptr_list, int *list_len)
{
    if (ptr == NULL)
        return ptr_list;

    ptr_list = realloc(ptr_list, (*list_len + 1) * sizeof (Node *));
    ptr_list[(*list_len)++] = ptr;

    ptr_list = preOrderTraversal(ptr->next[0], ptr_list, list_len);
    ptr_list = preOrderTraversal(ptr->next[1], ptr_list, list_len);
    ptr_list = preOrderTraversal(ptr->next[2], ptr_list, list_len);
    ptr_list = preOrderTraversal(ptr->next[3], ptr_list, list_len);

    return ptr_list;
}

Node *makeEmptyNode()
{
    Node *node = malloc(sizeof (Node));
    node->key_num = 0;
    for (int i = 0; i < sizeof (node->key) / sizeof (node->key[0]); i++)
        node->key[i] = NULL;
    for (int i = 0; i < sizeof (node->info) / sizeof (node->info[0]); i++)
        node->info[i] = NULL;
    for (int i = 0; i < sizeof (node->next) / sizeof (node->next[0]); i++)
        node->next[i] = NULL;
    return node;
}

Key_pos **inOrderTraversal(Node *ptr, Key_pos **kp_list, int *list_len)
{
    if (ptr == NULL)
        return kp_list;

    kp_list = inOrderTraversal(ptr->next[0], kp_list, list_len);

    if (ptr->key[0] != NULL)
    {
        Key_pos *pos = malloc(sizeof (Key_pos));
        pos->ptr = ptr;
        pos->index = 0;
        kp_list = realloc(kp_list, (*list_len + 1) * sizeof (Key_pos *));
        kp_list[(*list_len)++] = pos;
    }

    kp_list = inOrderTraversal(ptr->next[1], kp_list, list_len);

    if (ptr->key[1] != NULL)
    {
        Key_pos *pos = malloc(sizeof (Key_pos));
        pos->ptr = ptr;
        pos->index = 1;
        kp_list = realloc(kp_list, (*list_len + 1) * sizeof (Key_pos *));
        kp_list[(*list_len)++] = pos;
    }

    kp_list = inOrderTraversal(ptr->next[2], kp_list, list_len);

    if (ptr->key[2] != NULL)
    {
        Key_pos *pos = malloc(sizeof (Key_pos));
        pos->ptr = ptr;
        pos->index = 2;
        kp_list = realloc(kp_list, (*list_len + 1) * sizeof (Key_pos *));
        kp_list[(*list_len)++] = pos;
    }

    kp_list = inOrderTraversal(ptr->next[3], kp_list, list_len);

    return kp_list;
}

int delPostOrder(Node **pptr)
{
    if (*pptr == NULL)
        return 0;
    delPostOrder(&(*pptr)->next[3]);
    delPostOrder(&(*pptr)->next[2]);
    delPostOrder(&(*pptr)->next[1]);
    delPostOrder(&(*pptr)->next[0]);
    for (int i = 0; i < 3; i++)
    {
        if ((*pptr)->key[i] != NULL)
            free((*pptr)->key[i]), (*pptr)->key[i] = NULL;
        if ((*pptr)->info[i] != NULL)
            free((*pptr)->info[i]), (*pptr)->info[i] = NULL;
    }
    free(*pptr);
    return 0;
}

int bDeleteTree(Node **root)
{
    if (*root == NULL || root == NULL)
        return -1;
    if ((*root)->key_num == 0)
        return 1;
    delPostOrder(&(*root));
    *root = makeEmptyNode();
    return 0;
}

Node *processRoot(Node **root)
{
    if ((*root)->key_num == 1)
    {
        Node *y = (*root)->next[0];
        Node *z = (*root)->next[1];
        if (y->key_num == 1 && z->key_num == 1)
        {
            y->key[1] = (*root)->key[0];
            y->info[1] = (*root)->info[0];
            y->key[2] = z->key[0];
            y->info[2] = z->info[0];
            y->next[2] = z->next[0];
            z->next[3] = z->next[1];
            Node *ptr = *root;
            *root = y;
            free(z);
            free(ptr);
        }
    }
    return *root;
}

Node *isNeighbourKeyNumNotOne(const Node *parent, int i)
{
    if (i == 0)
        if (parent->next[1]->key_num > 1)
            return parent->next[1];
    if (i == 1 || i == 2)
    {
        if (parent->next[i + 1]->key_num > 1)
            return parent->next[i + 1];
        else if (parent->next[i - 1]->key_num > 1)
            return parent->next[i - 1];
        else
            return NULL;
    }
    if (i == 3)
        if (parent->next[2]->key_num > 1)
            return parent->next[2];
    return NULL;
}

Key_pos *findMax(Node *root)
{
    if (root == NULL)
        return NULL;
    Node *ptr = root;
    while (ptr->next[ptr->key_num] != NULL)
        ptr = ptr->next[ptr->key_num];
    Key_pos *kp = malloc(sizeof (Key_pos));
    kp->ptr = ptr;
    kp->index = ptr->key_num - 1;
    return kp;
}

Key_pos *findMin(Node *root)
{
    if (root == NULL)
        return NULL;
    Node *ptr = root;
    while (ptr->next[0] != NULL)
        ptr = ptr->next[0];
    Key_pos *kp = malloc(sizeof (Key_pos));
    kp->ptr = ptr;
    kp->index = 0;
    return kp;
}

int bDelete2(Node **root, const char *key)
{
    *root = processRoot(&(*root));
    Node *ptr = *root;
    while (ptr->next[0] != NULL)
    {
        printf("not list\n");
        int i;
        for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
        if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) != 0)
        {
            Node *y = ptr->next[i];
            if (y->key_num == 1 && isNeighbourKeyNumNotOne(ptr, i) != NULL)
            {
                Node *z = isNeighbourKeyNumNotOne(ptr, i);
                if (z == ptr->next[i + 1])
                {
                    y->key[1] = ptr->key[i];
                    y->info[1] = ptr->info[i];
                    ptr->key[i] = z->key[0];
                    ptr->info[i] = z->info[0];
                    y->next[2] = z->next[0];
                    memmove(&(z->key[0]), &(z->key[1]), sizeof (z->key[0]) * (z->key_num - 1));
                    memmove(&(z->info[0]), &(z->info[1]), sizeof (z->info[0]) * (z->key_num - 1));
                    memmove(&(z->next[0]), &(z->next[1]), sizeof (z->next[0]) * z->key_num);
                    z->key_num--;
                    if (z->key_num == 1)
                    {
                        z->key[2] = NULL, z->key[1] = NULL;
                        z->info[2] = NULL, z->info[2] = NULL;
                        z->next[3] = NULL, z->next[2] = NULL;
                    }
                    else if (z->key_num == 2)
                    {
                        z->key[2] = NULL;
                        z->info[2] = NULL;
                        z->next[3] = NULL;
                    }
                }
                else if (z == ptr->next[i - 1])
                {
                    memmove(&(y->key[1]), &(y->key[0]), sizeof (y->key[0]) * (y->key_num - 1));
                    memmove(&(y->info[1]), &(y->info[0]), sizeof (y->info[0]) * (y->key_num - 1));
                    memmove(&(y->next[1]), &(y->next[0]), sizeof (y->next[0]) * (y->key_num));
                    y->key[0] = ptr->key[i - 1];
                    y->info[0] = ptr->info[i - 1];
                    ptr->key[i - 1] = z->key[z->key_num - 1];
                    z->key[z->key_num - 1] = NULL;
                    ptr->info[i - 1] = z->info[z->key_num - 1];
                    z->info[z->key_num - 1] = NULL;
                    y->next[0] = z->next[z->key_num];
                    z->next[z->key_num] = NULL;
                    z->key_num--;
                }
                y->key_num++;
            }
            else if (y->key_num == 1 && isNeighbourKeyNumNotOne(ptr, i) == NULL)
            {
                Node *z;
                if (i != 3 && ptr->next[i + 1] != NULL)
                {
                    z = ptr->next[i + 1];
                    y->key[1] = ptr->key[i];
                    y->info[1] = ptr->info[i];
                    memmove(&(ptr->key[i]), &(ptr->key[i + 1]), sizeof (ptr->key[0]) * (ptr->key_num - 1));
                    memmove(&(ptr->info[i]), &(ptr->info[i + 1]), sizeof (ptr->info[0]) * (ptr->key_num - 1));
                    y->key[2] = z->key[0]; z->key[0] = NULL;
                    y->info[2] = z->info[0];
                    y->next[2] = z->next[0];
                    y->next[3] = z->next[1];
                    free(z);
                    memmove(&(ptr->next[i]), &(ptr->next[i + 1]), sizeof (ptr->next[0]) * (ptr->key_num));
                    ptr->key_num--;
                    y->key_num = 3;
                    if (ptr->key_num == 1)
                    {
                        ptr->key[2] = NULL; ptr->key[1] = NULL;
                        ptr->info[2] = NULL; ptr->info[1] = NULL;
                        ptr->next[3] = NULL; ptr->next[2] = NULL;
                    }
                    else if (ptr->key_num == 2)
                    {
                        ptr->key[2] = NULL;
                        ptr->info[2] = NULL;
                        ptr->next[3] = NULL;
                    }
                }
                else if (i == 3 && ptr->next[i - 1] != NULL)
                {
                    z = ptr->next[i - 1];
                    y->key[2] = y->key[0];
                    y->info[2] = y->info[0];
                    y->next[3] = y->next[1];
                    y->next[2] = y->next[0];
                    y->key[1] = ptr->key[i]; ptr->key[i] = NULL;
                    y->info[1] = ptr->info[i]; ptr->info[i] = NULL;
                    y->key[0] = z->key[0];
                    y->info[0] = z->info[0];
                    y->next[1] = z->next[1];
                    y->next[0] = z->next[0];
                    free(z);
                    ptr->next[i - 1] = y;
                    ptr->next[i] = NULL;
                    ptr->key_num--;
                    y->key_num = 3;
                }
            }
            ptr = y;
        }
        else if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
        {
            Node *y = ptr->next[i];
            Node *z = ptr->next[i + 1];
            if (y->key_num >= 2)
            {
                Key_pos *k = findMax(y);
                ptr->key[i] = realloc(ptr->key[i], strlen(k->ptr->key[k->index]) + 1);
                strcpy(ptr->key[i], k->ptr->key[k->index]);
                bDelete(&y, k->ptr->key[k->index]);
                free(k);
            }
            else if (z->key_num >= 2)
            {
                Key_pos *k = findMin(z);
                ptr->key[i] = realloc(ptr->key[i], strlen(k->ptr->key[k->index]) + 1);
                strcpy(ptr->key[i], k->ptr->key[k->index]);
                bDelete(&z, k->ptr->key[k->index]);
                free(k);
            }
            else
            {
                y->key[1] = ptr->key[i];
                y->info[1] = ptr->info[i];
                y->key[2] = z->key[0];
                y->info[2] = z->info[0];
                y->next[2] = z->next[0];
                y->next[3] = z->next[1];
                free(z);
                memmove(&(ptr->key[i]), &(ptr->key[i + 1]), sizeof (ptr->key[0]) * (ptr->key_num - 1));
                memmove(&(ptr->info[i]), &(ptr->info[i + 1]), sizeof (ptr->info[0]) * (ptr->key_num - 1));
                memmove(&(ptr->next[i]), &(ptr->next[i + 1]), sizeof (ptr->next[0]) * (ptr->key_num));
                ptr->key_num--;
                y->key_num = 3;
                if (ptr->key_num == 1)
                {
                    ptr->key[2] = NULL; ptr->key[1] = NULL;
                    ptr->info[2] = NULL; ptr->info[1] = NULL;
                    ptr->next[3] = NULL; ptr->next[2] = NULL;
                }
                else if (ptr->key_num == 2)
                {
                    ptr->key[2] = NULL;
                    ptr->info[2] = NULL;
                    ptr->next[3] = NULL;
                }
                bDelete(&y, key);
            }
        }
    }
    int i;
    for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
    if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
    {
        free(ptr->key[i]);
        free(ptr->info[i]);
        memmove(&(ptr->key[i]), &(ptr->key[i + 1]), sizeof (ptr->key[0]) * (ptr->key_num - 1));
        memmove(&(ptr->info[i]), &(ptr->info[i + 1]), sizeof (ptr->info[0]) * (ptr->key_num - 1));
        ptr->key_num--;
        if (ptr->key_num == 1)
        {
            ptr->key[2] = NULL; ptr->key[1] = NULL;
            ptr->info[2] = NULL; ptr->info[1] = NULL;
            ptr->next[3] = NULL; ptr->next[2] = NULL;
        }
        else if (ptr->key_num == 2)
        {
            ptr->key[2] = NULL;
            ptr->info[2] = NULL;
            ptr->next[3] = NULL;
        }
        return 0;
    }
    return 1;
}

Key_pos *bSearch(Node *root, const char *key)
{
    Node *ptr = root;
    while (ptr != NULL)
    {
        int i;
        for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
        if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
        {
            Key_pos *pos = malloc(sizeof (Key_pos));
            pos->ptr = ptr;
            pos->index = i;
            return pos;
        }
        else
            ptr = ptr->next[i];
    }
    return NULL;
}

Key_pos *bMoreSearch(Node *root, const char *key)
{
    Node *ptr = root;
    while (1)
    {
        int i;
        for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
        if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
        {
            Key_pos *pos = malloc(sizeof (Key_pos));
            pos->ptr = ptr;
            pos->index = i;
            return pos;
        }
        else
        {
            if (ptr->next[i] != NULL)
                ptr = ptr->next[i];
            else
            {
                Key_pos *pos = malloc(sizeof (Key_pos));
                pos->ptr = ptr;
                if (i == 3)
                    i--;
                pos->index = i;
                return pos;
            }
        }
    }
    return NULL;
}

int bDelete(Node **root, const char *key)
{
    Key_pos **kp_list = NULL;
    int list_len = 0;
    kp_list = inOrderTraversal(*root, kp_list, &list_len);
    for (int i = 0; i < list_len; i++)
    {
        if (strcmp(kp_list[i]->ptr->key[kp_list[i]->index], key) == 0)
        {
            free(kp_list[i]->ptr->key[kp_list[i]->index]);
            free(kp_list[i]->ptr->info[kp_list[i]->index]);
            //for (int i = 0; i < list_len; i++)
                //printf("Key: %s | Info: %d\n", kp_list[i]->ptr->key[kp_list[i]->index], *kp_list[i]->ptr->info[kp_list[i]->index]);
            memmove(&(kp_list[i]), &(kp_list[i + 1]), sizeof (Key_pos *) * (list_len - i - 1));
            //printf("|\n");
            //kp_list[list_len - 1]->ptr->key[kp_list[list_len - 1]->index] = NULL;
            //kp_list[list_len - 1]->ptr->info[kp_list[list_len - 1]->index] = NULL;
            kp_list[list_len - 1] = NULL;
            list_len--;
            //for (int i = 0; i < list_len; i++)
                //printf("Key: %s | Info: %d\n", kp_list[i]->ptr->key[kp_list[i]->index], *kp_list[i]->ptr->info[kp_list[i]->index]);
            //bDeleteTree(&(*root));
            *root = makeEmptyNode();
            for (int j = 0; j < list_len; j++)
                bInsert(&(*root), kp_list[j]->ptr->key[kp_list[j]->index], *kp_list[j]->ptr->info[kp_list[j]->index]);
            free(kp_list);
            return 0;
        }
    }
    free(kp_list);
    //system("pause");
    return 1;
}

int bSplit(Node *parent, int i)
{
    Node *y = parent->next[i];
    if (parent->key_num == MAX_KEY_NUM)
    {
        printf("error split\n");
        return -1;
    }
    memmove(&(parent->key[i + 1]), &(parent->key[i]), sizeof (parent->key[0]) * (parent->key_num - i));
    memmove(&(parent->info[i + 1]), &(parent->info[i]), sizeof (parent->info[0]) * (parent->key_num - i));
    memmove(&(parent->next[i + 2]), &(parent->next[i + 1]), sizeof (parent->next[0]) * (parent->key_num - i));
    Node *z = makeEmptyNode();
    memcpy(&(z->key[0]), &(y->key[LAST_KEY_INDEX]), sizeof (z->key[0]));
    memcpy(&(z->info[0]), &(y->info[LAST_INFO_INDEX]), sizeof (z->info[0]));
    memcpy(&(z->next[0]), &(y->next[LAST_KEY_INDEX]), sizeof (z->next[0]));
    memcpy(&(z->next[1]), &(y->next[LAST_KEY_INDEX + 1]), sizeof (z->next[0]));
    z->key_num = 1;
    parent->next[i + 1] = z;
    parent->key[i] = y->key[1];
    parent->info[i] = y->info[1];
    parent->key_num++;
    //printf("par key num: %d\n", parent->key_num);
    y->key[1] = NULL;
    y->key[2] = NULL;
    y->info[1] = NULL;
    y->info[2] = NULL;
    y->next[2] = NULL;
    y->next[3] = NULL;
    y->key_num = 1;
    //printf("-----\n");
    //printf("z: %s\n", z->key[0]);
    //printf("par: %s\n", parent->key[0]);
    //printf("y: %s\n", y->key[0]);
    //printf("-----\n");
    //system("pause");
    return 0;
}

int bInsertNonFull(Node *node, const char *key, Offset info);

/*int bInsert(B_Tree *tree, const char *key, unsigned info)
{
    if (tree == NULL)
        return -1;
    if (key == NULL)
        return -2;
    Node *ptr = tree->root;
    if (tree->root->key_num == MAX_KEY_NUM)
    {
        Node *new_root = makeEmptyNode();
        new_root->next[0] = ptr;
        //ptr->parent = new_root;
        tree->root = new_root;
        bSplit(new_root, 0);
        printf("--------------------\n");
        printTree(tree->root, 0);
        printf("--------------------\n");
        ptr = new_root;
    }
    printf("ins non full\n");
    return bInsertNonFull(ptr, key, info);
}*/

int bInsert(Node **root, const char *key, Offset info)
{
    if (*root == NULL)
        return -1;
    if (key == NULL)
        return -2;
    Node *ptr = *root;
    if ((*root)->key_num == MAX_KEY_NUM)
    {
        Node *new_root = makeEmptyNode();
        new_root->next[0] = ptr;
        //ptr->parent = new_root;
        *root = new_root;
        bSplit(new_root, 0);
        //printf("--------------------\n");
        //printTree(*root, 0);
        //printf("--------------------\n");
        ptr = new_root;
    }
    //printf("ins non full\n");
    return bInsertNonFull(ptr, key, info);
}

int bInsertNonFull(Node *node, const char *key, Offset info)
{
    Node *ptr = node;
    while (ptr->next[0] != NULL)
    {
        int i;
        //printf("null1\n");
        for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
        //printf("null2\n");

        if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
            return 1;
        Node *s = ptr->next[i];
        if (s->key_num == MAX_KEY_NUM)
        {
            bSplit(ptr, i);
            if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) > 0)
                s = ptr->next[i + 1];
        }
        ptr = s;
        //printf("ptr->key[0]: %s\n", ptr->key[0]);
    }
    int i;
    for (i = 0; i < ptr->key_num && strcmp(key, ptr->key[i]) > 0; i++);
    if (ptr->key[i] != NULL && strcmp(key, ptr->key[i]) == 0)
        return 1;
    memmove(&(ptr->key[i + 1]), &(ptr->key[i]), sizeof (ptr->key[0]) * (ptr->key_num - i));
    memmove(&(ptr->info[i + 1]), &(ptr->info[i]), sizeof (ptr->info[0]) * (ptr->key_num - i));
    ptr->key[i] = malloc(strlen(key) + 1);
    strcpy(ptr->key[i], key);
    ptr->info[i] = malloc(sizeof (Offset ));
    *ptr->info[i] = info;
    ptr->key_num++;
    return 0;
}

int printTree(const Node *ptr, int space)
{
    if (ptr == NULL)
    {
        //printf("\n");
        return 0;
    }
    printTree(ptr->next[3], space + 12);
    if (ptr->key[2] != NULL)
    {
        for (int i = 0; i < space; i++) printf(" ");
        printf("key:\"%s\"\n", ptr->key[2]);
    }
    printTree(ptr->next[2], space + 12);
    if (ptr->key[1] != NULL)
    {
        for (int i = 0; i < space; i++) printf(" ");
        printf("key:\"%s\"\n", ptr->key[1]);
    }
    printTree(ptr->next[1], space + 12);
    if (ptr->key[0] != NULL)
    {
        for (int i = 0; i < space; i++) printf(" ");
        printf("key:\"%s\"\n", ptr->key[0]);
    }
    printTree(ptr->next[0], space + 12);
    return 0;
}
