#include "b-tree.h"
#include "dialog.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int _mystrcmp(const char * s1, const char * s2)
{
    for (; *s1 == *s2; ++s1, ++s2)
        if (*s1 == 0)
            return 0;
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

int mystrcmp(const char* s1, const char* s2)
{
    while(*s1 && *s1 == *s2)
        s1++, s2++;
    return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}

int dialog(const char *msgs[], const int NMsgs)
{
    system("cls");
    for (int i = 0; i < NMsgs; i++)
        printf("%s\n", msgs[i]);
    printf("\nEnter the alternative\n");
    int flag;
    int temp;
    do
    {
        printf(">");
        flag = scanf("%d", &temp);
        if (flag < 0)
            return 0;
        if (flag == 0 || temp >= NMsgs)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1 || temp >= NMsgs);
    scanf("%*[^\n]");
    scanf("%*c");
    return temp;
}

char *getStr()
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do
    {
        n = scanf("%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
                return NULL;
        }
        else if (n > 0)
        {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
            scanf("%*c");
    } while (n > 0);
    if (len > 0)
        res[len] = '\0';
    else
    {
        free(res);
        res = calloc(1, sizeof (char));
    }
    return res;
}

char *fgetStr(FILE *file)
{
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do
    {
        n = fscanf(file, "%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
                return NULL;
        }
        else if (n > 0)
        {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else
            fscanf(file, "%*c");
    } while (n > 0);
    if (len > 0)
        res[len] = '\0';
    else
    {
        free(res);
        res = calloc(1, sizeof (char));
    }
    return res;
}

int getUINT(unsigned *value)
{
    int flag;
    int temp;
    do
    {
        flag = scanf("%d", &temp);
        if (flag < 0)
            return -1;
        if (flag == 0 || temp < 0)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1 || temp < 0);
    *value = temp;
    return 0;
}

char *addExtension(const char *file_name, const char *extension)
{
    if (file_name == NULL || extension == NULL)
        return NULL;
    char *file_name_extension = malloc(strlen(file_name) + strlen(extension) + 1);
    strcpy(file_name_extension, file_name);
    strcat(file_name_extension, extension);
    return file_name_extension;
}

int D_Add(B_Tree *tree)
{
    if (tree == NULL)
        return -1;
    printf("Enter key (null terminated string): ");
    char *key = getStr();
    if (key == NULL)
        return 1;
    printf("Enter information (unsigned integer): ");
    unsigned info;
    if (getUINT(&info) < 0)
    {
        free(key);
        return 1;
    }
    //scanf("%*c");
    int r = bInsert(&tree->root, key, info);
    if (r == 1)
        printf("Duplicate key\n");
    getchar();
    return 0;
}

int D_Delete(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter key (null terminated string): ");
    char *key = getStr();
    if (key == NULL)
        return 3;
    if (bSearch(tree->root, key) == NULL)
    {
        printf("No key found\n");
        free(key);
        getchar();
        return 2;
    }
    bDelete(&tree->root, key);
    return 0;
}

int D_Traversal(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter maximum key (null terminated string): ");
    char *key = getStr();
    Key_pos **kp_list = NULL;
    int list_len = 0;
    kp_list = inOrderTraversal(tree->root, kp_list, &list_len);
    //printf("list_len %d\n", list_len);
    //printf("root %s | %d\n", tree->root->key[0], *tree->root->info[0]);
    int i;
    for (i = 0; i < list_len; i++)
        if (key != NULL)
        {
            if (strcmp(kp_list[i]->ptr->key[kp_list[i]->index], key) <= 0)
                printf("Key: %s | Info: %d\n", kp_list[i]->ptr->key[kp_list[i]->index], *kp_list[i]->ptr->info[kp_list[i]->index]);
            else
                break;
        }
        else
            printf("Key: %s | Info: %d\n", kp_list[i]->ptr->key[kp_list[i]->index], *kp_list[i]->ptr->info[kp_list[i]->index]);
    printf("len: %d\n", i);
    if (key != NULL)
        free(key);
    getchar();
    return 0;
}

int D_Find(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("1. Find by key\n"
           "2. Find keys that are close in value\n"
           ">");
    unsigned choice;
    if (getUINT(&choice) < 0)
        return 1;
    scanf("%*c");
    if (choice == 1)
    {
        printf("Enter key (null terminated string): ");
        char *key = getStr();
        if (key == NULL)
            return 1;
        Key_pos *pos = bSearch(tree->root, key);
        if (pos != NULL)
            printf("Key: %s | Info: %d\n", pos->ptr->key[pos->index], *pos->ptr->info[pos->index]);
        else
            printf("No key found\n");
        free(key);
        getchar();
    }
    else if (choice == 2)
    {
        printf("Enter key (null terminated string): ");
        char *key = getStr();
        if (key == NULL)
            return 1;
        /*Key_pos *kp = bSearch(tree->root, key);
        if (kp != NULL)
        {
            Key_pos *less = findMax(kp->ptr->next[kp->index]);
            Key_pos *more = findMin(kp->ptr->next[kp->index + 1]);
            if (less == NULL && more != NULL)
                printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
            if (more == NULL && less != NULL)
                printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
            if (more == NULL && less == NULL)
            {
                printf("No key found\n");
                getchar();
                free(key);
                return 1;
            }
            if (more != NULL && less != NULL)
            {
                if (abs(mystrcmp(less->ptr->key[less->index], key)) < abs(mystrcmp(more->ptr->key[more->index], key)))
                    printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
                else if (abs(mystrcmp(less->ptr->key[less->index], key)) > abs(mystrcmp(more->ptr->key[more->index], key)))
                    printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
                else
                {
                    printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
                    printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
                }
            }
        }
        else
        {
            Key_pos *more = bMoreSearch(tree->root, key);
            Key_pos *less = NULL;
            if (more != NULL)
                less = findMax(more->ptr->next[more->index]);
            if (less == NULL && more != NULL)
                printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
            if (more == NULL && less != NULL)
                printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
            if (more == NULL && less == NULL)
            {
                printf("No key found\n");
                getchar();
                free(key);
                return 1;
            }
            if (more != NULL && less != NULL)
            {
                if (abs(mystrcmp(less->ptr->key[less->index], key)) < abs(mystrcmp(more->ptr->key[more->index], key)))
                    printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
                else if (abs(mystrcmp(less->ptr->key[less->index], key)) > abs(mystrcmp(more->ptr->key[more->index], key)))
                    printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
                else
                {
                    printf("Key: %s | Info: %d\n", less->ptr->key[less->index], *less->ptr->info[less->index]);
                    printf("Key: %s | Info: %d\n", more->ptr->key[more->index], *more->ptr->info[more->index]);
                }
            }
        }*/

        Key_pos **kp_list = NULL;
        int list_len = 0;
        kp_list = inOrderTraversal(tree->root, kp_list, &list_len);
        if (list_len == 0)
        {
            printf("No key found\n");
            free(key);
            getchar();
            return 1;
        }
        int i;
        for (i = 0; i < list_len && mystrcmp(key, kp_list[i]->ptr->key[kp_list[i]->index]) > 0; i++);
        //printf("%d\n", i);

        if (i != list_len && mystrcmp(key, kp_list[i]->ptr->key[kp_list[i]->index]) == 0)
        {
            if (list_len == 1)
                printf("No key found\n");
            else if (i == 0)
                printf("Key: %s | Info: %d\n",
                       kp_list[i + 1]->ptr->key[kp_list[i + 1]->index],
                       *kp_list[i + 1]->ptr->info[kp_list[i + 1]->index]);
            else if (i == list_len - 1)
                printf("Key: %s | Info: %d\n",
                       kp_list[i - 1]->ptr->key[kp_list[i - 1]->index],
                       *kp_list[i - 1]->ptr->info[kp_list[i - 1]->index]);
            else
            {
//                printf("%d %d\n", abs(mystrcmp(kp_list[i - 1]->ptr->key[kp_list[i - 1]->index], key)),
//                                      abs(mystrcmp(kp_list[i + 1]->ptr->key[kp_list[i + 1]->index], key)));
                int res = abs(mystrcmp(kp_list[i - 1]->ptr->key[kp_list[i - 1]->index], key)) -
                              abs(mystrcmp(kp_list[i + 1]->ptr->key[kp_list[i + 1]->index], key));
                if (res == 0)
                {
                    printf("Key: %s | Info: %d\n",
                           kp_list[i - 1]->ptr->key[kp_list[i - 1]->index],
                           *kp_list[i - 1]->ptr->info[kp_list[i - 1]->index]);
                    printf("Key: %s | Info: %d\n",
                           kp_list[i + 1]->ptr->key[kp_list[i + 1]->index],
                           *kp_list[i + 1]->ptr->info[kp_list[i + 1]->index]);
                }
                else if (res > 0)
                    printf("Key: %s | Info: %d\n",
                           kp_list[i + 1]->ptr->key[kp_list[i + 1]->index],
                           *kp_list[i + 1]->ptr->info[kp_list[i + 1]->index]);
                else if (res < 0)
                    printf("Key: %s | Info: %d\n",
                           kp_list[i - 1]->ptr->key[kp_list[i - 1]->index],
                           *kp_list[i - 1]->ptr->info[kp_list[i - 1]->index]);
            }
        }
        else
        {
            if (i == list_len)
                i--;
            if (list_len == 1 || i == 0)
            {
                printf("Key: %s | Info: %d\n",
                       kp_list[0]->ptr->key[kp_list[0]->index],
                       *kp_list[0]->ptr->info[kp_list[0]->index]);
                free(key);
                getchar();
                return 1;
            }
            int res = abs(mystrcmp(kp_list[i - 1]->ptr->key[kp_list[i - 1]->index], key)) -
                      abs(mystrcmp(kp_list[i]->ptr->key[kp_list[i]->index], key));

            if (res < 0)
                printf("Key: %s | Info: %d\n",
                       kp_list[i - 1]->ptr->key[kp_list[i - 1]->index],
                       *kp_list[i - 1]->ptr->info[kp_list[i - 1]->index]);
            else if (res > 0)
                printf("Key: %s | Info: %d\n",
                       kp_list[i]->ptr->key[kp_list[i]->index],
                       *kp_list[i]->ptr->info[kp_list[i]->index]);
            else
            {
                printf("Key: %s | Info: %d\n",
                       kp_list[i - 1]->ptr->key[kp_list[i - 1]->index],
                       *kp_list[i - 1]->ptr->info[kp_list[i - 1]->index]);
                printf("Key: %s | Info: %d\n",
                       kp_list[i]->ptr->key[kp_list[i]->index],
                       *kp_list[i]->ptr->info[kp_list[i]->index]);
            }
        }
        free(key);
        getchar();
    }
    return 0;
}

int D_Show(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
        printf("Tree is empty...\n");
    else
        printTree(tree->root, 0);
    getchar();
    return 0;
}

char *makeCommand(const char *gv_file, const char *png_file)
{
    if (gv_file == NULL || png_file == NULL)
        return NULL;
    char *command = malloc(strlen("D:\\Programs\\Graphviz\\bin\\dot ")
                           + strlen(gv_file) + strlen(" -Tpng -o ") + strlen(png_file) + 1);
    strcpy(command, "D:\\Programs\\Graphviz\\bin\\dot ");
    strcat(command, gv_file);
    strcat(command, " -Tpng -o ");
    strcat(command, png_file);
    return command;
}

typedef struct FileNode
{
    Node *node;
    char var[16];
} FileNode;

int writeToGV(B_Tree *tree, FILE *file, const char *file_name)
{
    fseek(file, 0, SEEK_SET);
    fprintf(file, "digraph %s\n"
                  "{\n", file_name);
    fprintf(file, "node [shape = record, height = 0.1, fontname = consolas];\n");
    Node **ptr_list = NULL;
    int len = 0;
    ptr_list = preOrderTraversal(tree->root, ptr_list, &len);
    char buf[16];
    FileNode *node_list = NULL;
    int list_len = 0;
    for (int i = 0; i < len; i++)
    {
        FileNode file_node;
        file_node.node = ptr_list[i];
        itoa(i, buf, 10);
        char *var = addExtension("node", buf);
        strcpy(file_node.var, var);
        free(var);
        fprintf(file, "%s [label = \"<f0> ", file_node.var);
        for (int i = 0; i < file_node.node->key_num; i++)
            fprintf(file, "|Key: %s\\nInfo: %d| <f%d> ", file_node.node->key[i], *file_node.node->info[i], i + 1);
        fprintf(file, "\"];\n");
        node_list = realloc(node_list, (list_len + 1) * sizeof (FileNode));
        node_list[list_len++] = file_node;
    }
    fprintf(file, "\n");
    for (int i = 0; i < list_len; i++)
    {
        if (node_list[i].node->next[0] != NULL)
        {
            fprintf(file, "%s : f0 -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->next[0])
                {
                    fprintf(file, "%s;\n", node_list[j].var);
                    break;
                }
        }
        if (node_list[i].node->next[1] != NULL)
        {
            fprintf(file, "%s : f1 -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->next[1])
                {
                    fprintf(file, "%s;\n", node_list[j].var);
                    break;
                }
        }
        if (node_list[i].node->next[2] != NULL)
        {
            fprintf(file, "%s : f2 -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->next[2])
                {
                    fprintf(file, "%s;\n", node_list[j].var);
                    break;
                }
        }
        if (node_list[i].node->next[3] != NULL)
        {
            fprintf(file, "%s : f3 -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->next[3])
                {
                    fprintf(file, "%s;\n", node_list[j].var);
                    break;
                }
        }
//        if (node_list[i].node->next != NULL &&
//            node_list[i].node->next != node_list[i].node->left &&
//            node_list[i].node->next != node_list[i].node->right && use_threads != 0)
//        {
//            fprintf(file, "  %s -> ", node_list[i].var);
//            for (int j = 0; j < list_len; j++)
//                if (node_list[j].node == node_list[i].node->next)
//                {
//                    fprintf(file, "%s [color = \"#00ff00\", fontcolor = \"#00ff00\"];\n", node_list[j].var);
//                    break;
//                }
//        }
    }
    free(node_list);
    fprintf(file, "}");
    return 0;
}

int D_saveToPng(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_gv = addExtension(file_name, ".gv");
    FILE *file;
    file = fopen(file_name_gv, "w");

    writeToGV(tree, file, file_name);

    fclose(file);
    char *file_name_png = addExtension(file_name, ".png");
    char *command = makeCommand(file_name_gv, file_name_png);
    //printf("%s\n", command);
    system(command), free(command);
    system(file_name_png);
    free(file_name), free(file_name_gv), free(file_name_png);
    printf("Success\n");
    getchar();
    return 0;
}

int writeToTxt(B_Tree *tree, FILE *file)
{
    if (tree == NULL || tree->root == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    Key_pos **kp_list = NULL;
    int list_len = 0;
    kp_list = inOrderTraversal(tree->root, kp_list, &list_len);
    for (int i = 0; i < list_len; i++)
    {
        fprintf(file, "%s\n", kp_list[i]->ptr->key[kp_list[i]->index]);
        fprintf(file, "%d\n", *kp_list[i]->ptr->info[kp_list[i]->index]);
    }
    return 0;
}

int D_saveToTxt(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    if (tree->root->key_num == 0)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_txt = addExtension(file_name, ".txt");
    FILE *file;
    file = fopen(file_name_txt, "w");

    writeToTxt(tree, file);

    fclose(file);
    free(file_name), free(file_name_txt);
    printf("Success\n");
    getchar();
    return 0;
}

int readFromTxt(B_Tree *tree, FILE *file)
{
    if (tree == NULL || tree->root == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    char *key;
    while ((key = fgetStr(file)) != NULL)
    {
        unsigned info;
        fscanf(file, "%d", &info);
        fscanf(file, "%*c");
        bInsert(&tree->root, key, info);
        free(key);
    }
    return 0;
}

int D_loadFromTxt(B_Tree *tree)
{
    if (tree == NULL || tree->root == NULL)
        return -1;
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    char *file_name_txt = addExtension(file_name, ".txt");
    FILE *file;
    file = fopen(file_name_txt, "r");
    if (file == NULL)
    {
        perror("Error occured while opening file");
        free(file_name), free(file_name_txt);
        getchar();
        return 1;
    }
    if (tree->root != NULL)
        bDeleteTree(&tree->root);

    readFromTxt(tree, file);

    fclose(file);
    free(file_name), free(file_name_txt);
    printf("Success\n");
    //getchar();
    return 0;
}


char *makeRandomKey()
{
    char *str = malloc(6);
    for (int i = 0; i < 5; i++)
        str[i] = rand() % 26 + 97;
    str[5] = '\0';
    return str;
}

int D_Timing(B_Tree *tree)
{
    srand(time(NULL));
//    printf("insert:\n");
//    int duplicates = 0;
//    for (int i = 500000; i <= 10000000; i+= 500000)
//    {
//        for (int j = 0; j < i; j++)
//        {
//            char *key = makeRandomKey();
//            if (bInsert(&tree->root, key, rand()) == 1)
//                duplicates++;
//            free(key);
//        }
//        time_t start = clock();
//        for (int n = 0; n < 100000; n++)
//        {
//            char *key = makeRandomKey();
//            if (bInsert(&tree->root, key, rand()) == 1)
//                duplicates++;
//            free(key);
//        }
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        bDeleteTree(&tree->root);
//    }
//    printf("Duplicates: %d\n", duplicates);
//    printf("\n");
//    printf("search:\n");
//    int not_found = 0, duplicates = 0;
//    for (int i = 500000; i <= 10000000; i+= 500000)
//    {
//        for (int j = 0; j < i; j++)
//        {
//            char *key = makeRandomKey();
//            if (bInsert(&tree->root, key, rand()) == 1)
//                duplicates++;
//            free(key);
//        }
//        time_t start = clock();
//        for (int n = 0; n < 100000; n++)
//        {
//            char *key = makeRandomKey();
//            if (bSearch(tree->root, key) == NULL)
//                not_found++;
//            free(key);
//        }
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        bDeleteTree(&tree->root);
//    }
//    printf("Duplicates: %d\n", duplicates);
//    printf("Not found: %d\n", not_found);
//    printf("delete:\n");
//    int not_found = 0, duplicates = 0;
//    for (int i = 10000; i <= 100000; i+= 10000)
//    {
//        for (int j = 0; j < i; j++)
//        {
//            char *key = makeRandomKey();
//            if (bInsert(&tree->root, key, rand()) == 1)
//                duplicates++;
//            free(key);
//        }
//        time_t start = clock();
//        for (int n = 0; n < 1000; n++)
//        {
//            char *key = makeRandomKey();
//            if (bDelete(&tree->root, key) == 1)
//                not_found++;
//            free(key);
//        }
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        bDeleteTree(&tree->root);
//    }
//    printf("Duplicates: %d\n", duplicates);
//    printf("Not found: %d\n", not_found);
//    printf("traversal:\n");
    int duplicates = 0;
    int list_len;
    for (int i = 500000; i <= 10000000; i+= 500000)
    {
        for (int j = 0; j < i; j++)
        {
            char *key = makeRandomKey();
            if (bInsert(&tree->root, key, rand()) == 1)
                duplicates++;
            free(key);
        }
        time_t start = clock();
        //Key_pos **kp_list = NULL;
        list_len = 0;
        timingInOrderTraversal(tree->root, &list_len);
        time_t end = clock();
        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
        //free(kp_list);
        bDeleteTree(&tree->root);
    }
    printf("list_len: %d", list_len);
    printf("Duplicates: %d\n", duplicates);
    printf("\n");
    getchar();
    return 0;
}

int D_Random(B_Tree *tree)
{
    tree->root = makeEmptyNode();
    printf("Enter number of keys: ");
    unsigned num;
    if (getUINT(&num) < 0)
        return 1;
    srand(time(NULL));
    int duplicates = 0;
    time_t start = clock();
    for (int n = 0; n < num; n++)
    {
        char *key = makeRandomKey();
        if (bInsert(&tree->root, key, rand()) == 1)
            duplicates++;
        free(key);
    }
    time_t end = clock();
    printf("Duplicates: %d\n", duplicates);
    printf("time: %lf\n", (double) (end - start) / CLOCKS_PER_SEC);
    scanf("%*c");
    getchar();
    //bDeleteTree(&tree->root);
    return 0;
}

char *increment(char *str)
{
    if (strcmp(str, "zz") == 0)
        return NULL;
    if (str[1] < 'z' && str[1] >= 96)
        str[1]++;
    else if (str[1] == 'z')
    {
        str[0]++;
        str[1] = 'a';
    }
    return str;
}
