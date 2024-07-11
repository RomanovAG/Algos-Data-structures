#include "tree.h"
#include "dialog.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

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
        res = calloc(1, sizeof(char));
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
        res = calloc(1, sizeof(char));
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

int D_Add(Tree *tree)
{
    if (tree == NULL)
        return -1;
    printf("Enter key (unsigned integer): ");
    key_t key;
    if (getUINT(&key) < 0)
        return 1;
    scanf("%*c");
    printf("Enter information: ");
    char *info = getStr();
    if (info == NULL)
        return 1;
    if (insert(tree, makeNode(key, info)) == 0)
    {
//        Node **ptr_list = NULL;
//        int list_len = 0;
//        ptr_list = preOrderTraversion(tree->root, ptr_list, &list_len);
//        for (int i = 0; i < list_len - 1; i++)
//            ptr_list[i]->next = ptr_list[i + 1];
//        free(ptr_list);
        printf("Success\n");
//        printf("Traversal: ");
//        //int i = 0;
//        Node **list = NULL;
//        for (Node *ptr = tree->root; ptr != NULL; ptr = ptr->next)
//            printf("%d ", ptr->key);
    }
    else
        printf("Error\n");
    free(info);
    getchar();
    return 0;
}

int D_Traversal(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter range to ignore: ");
    key_t key1, key2;
    scanf("%d", &key1);
    scanf("%d", &key2);
    for (Node *ptr = findFirstMin(tree->root); ptr != NULL; ptr = ptr->next)
        if ((ptr->key < MIN(key1, key2) || ptr->key > MAX(key1, key2)))
            printf("Key: %d | Info: %s\n", ptr->key, ptr->info);
    scanf("%*c");
    getchar();
    return 0;
}

int D_Find(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("1. Find by key\n"
           "2. Find maximum key\n>");
    unsigned choice;
    if (getUINT(&choice) < 0)
        return 1;
    if (choice == 1)
    {
        printf("Enter key: ");
        key_t key;
        if (getUINT(&key) < 0)
            return 1;
        Node **nodes = find(tree->root, key);
        for (int i = 0; nodes[i] != NULL; i++)
            printf("Key: %d | Info: %s\n", nodes[i]->key, nodes[i]->info);
        scanf("%*c");
        getchar();
    }
    else if (choice == 2)
    {
        Node **nodes = findMax(tree->root);
        for (int i = 0; nodes[i] != NULL; i++)
            printf("Key: %d | Info: %s\n", nodes[i]->key, nodes[i]->info);
        scanf("%*c");
        getchar();
    }
    return 0;
}

int D_Delete(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter key to delete: ");
    key_t key;
    if (getUINT(&key) < 0)
        return 1;
    Node *to_delete = findFirst(tree->root, key);
    if (to_delete == NULL)
    {
        printf("No such node...\n");
        scanf("%*c");
        getchar();
        return 1;
    }
    delete(tree, findFirst(tree->root, key));
    return 0;
}

int printTree(const Node *ptr, int space)
{
    if (ptr == NULL)
        return 0;
    printTree(ptr->right, space + 8);
    for (int i = 0; i < space; i++)
        printf(" ");
    printf("%d\n", ptr->key);
    printTree(ptr->left, space + 8);
    return 0;
}

int D_Show(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
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

typedef struct FileNode
{
    Node *node;
    char var[3];
} FileNode;

int writeToGV(Tree *tree, FILE *file, const char *file_name, int use_threads)
{
    fseek(file, 0, SEEK_SET);
    fprintf(file, "digraph %s\n"
                  "{\n", file_name);
    char *var1 = malloc(3);
    strcpy(var1, "aa");
    var1[1] = 96;
    //Node *ptr = tree->root;
    int len = 0;
    Node **ptr_list = NULL;
    ptr_list = preOrderTraversal(tree->root, ptr_list, &len);
    Node *ptr = ptr_list[0];
    FileNode *node_list = NULL;
    int list_len = 0;
    for (int i = 1; i <= len && ptr != NULL; i++)
    {
        FileNode file_node;
        file_node.node = ptr;
        strcpy(file_node.var, increment(var1));
        if (file_node.node != tree->root)
            fprintf(file, "%s [label = \"%d\\n%s\", fontname = \"Consolas\"];\n",
                    file_node.var, file_node.node->key, file_node.node->info);
        else
            fprintf(file, "%s [label = \"%d\\n%s\", shape = box, fontname = \"Consolas\"];\n",
                    file_node.var, file_node.node->key, file_node.node->info);
        node_list = realloc(node_list, (list_len + 1) * sizeof (FileNode));
        node_list[list_len++] = file_node;
        ptr = ptr_list[i];
    }
    free(var1);
    fprintf(file, "\n");
    for (int i = 0; i < list_len; i++)
    {
        if (node_list[i].node->left != NULL)
        {
            fprintf(file, "%s -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->left)
                {
                    fprintf(file, "%s [label = \"L\", color = \"#0000ff\", fontcolor = \"#0000ff\", fontname = \"Consolas\"];\n",
                            node_list[j].var);
                    break;
                }
        }
        if (node_list[i].node->right != NULL)
        {
            fprintf(file, "%s -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->right)
                {
                    fprintf(file, "%s [label = \"R\", color = \"#ff0000\", fontcolor = \"#ff0000\", fontname = \"Consolas\"];\n",
                            node_list[j].var);
                    break;
                }
        }
        if (node_list[i].node->next != NULL &&
            node_list[i].node->next != node_list[i].node->left &&
            node_list[i].node->next != node_list[i].node->right && use_threads != 0)
        {
            fprintf(file, "  %s -> ", node_list[i].var);
            for (int j = 0; j < list_len; j++)
                if (node_list[j].node == node_list[i].node->next)
                {
                    fprintf(file, "%s [color = \"#00ff00\", fontcolor = \"#00ff00\"];\n", node_list[j].var);
                    break;
                }
        }
    }
    free(node_list);
    fprintf(file, "}");
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

int D_saveToPng(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
    {
        printf("Tree is empty...\n");
        getchar();
        return 1;
    }
    printf("Enter file name without extension: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return 1;
    printf("Use threads? (0/1): ");
    unsigned use_threads;
    if (getUINT(&use_threads) < 0)
        return 1;
    char *file_name_gv = addExtension(file_name, ".gv");
    FILE *file;
    file = fopen(file_name_gv, "w");

    writeToGV(tree, file, file_name, use_threads);

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

int writeToTxt(Tree *tree, FILE *file)
{
    if (tree == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    Node *ptr = findFirstMin(tree->root);
    while (ptr != NULL)
    {
        fprintf(file, "%d\n", ptr->key);
        fprintf(file, "%s\n", ptr->info);
        ptr = ptr->next;
    }
    return 0;
}

int D_saveToTxt(Tree *tree)
{
    if (tree == NULL)
        return -1;
    if (tree->root == NULL)
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

int readFromTxt(Tree *tree, FILE *file)
{
    if (tree == NULL || file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    key_t key;
    while (fscanf(file, "%d", &key) != EOF)
    {
        fscanf(file, "%*c");
        char *info = fgetStr(file);
        insert(tree, makeNode(key, info));
        free(info);
    }
    return 0;
}

int D_loadFromTxt(Tree *tree)
{
    if (tree == NULL)
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
        deleteTree(tree);
    readFromTxt(tree, file);
    fclose(file);
    free(file_name), free(file_name_txt);
    printf("Success\n");
    getchar();
    return 0;
}

int D_Timing(Tree *tree)
{
    srand(time(NULL));
//    printf("insert:\n");
//    for (int i = 50000; i <= 1000000; i+= 50000)
//    {
//        //printf("i = %d\n", i);
//        for (int j = 0; j < i; j++)
//            insert(tree, makeNode(rand(), ""));
//        time_t start = clock();
//        for (int n = 0; n < 1000; n++)
//            insert(tree, makeNode(rand(), ""));
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        deleteTree(tree);
//    }
//    printf("\n");
//    printf("delete:\n");
//    for (int i = 50000; i <= 1000000; i+= 50000)
//    {
//        //printf("i = %d\n", i);
//        for (int j = 0; j < i; j++)
//            insert(tree, makeNode(rand() % 1000, ""));
//        time_t start = clock();
//        for (int n = 0; n < 1000; n++)
//        {
//            //printf("0");
//            delete(tree, makeNode(rand() % 1000, ""));
//        }
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        deleteTree(tree);
//    }
//    printf("find:\n");
//    for (int i = 50000; i <= 1000000; i+= 50000)
//    {
//        //printf("i = %d\n", i);
//        for (int j = 0; j < i; j++)
//            insert(tree, makeNode(rand(), ""));
//        time_t start = clock();
//        for (int n = 0; n < 1000; n++)
//        {
//            find(tree->root, rand());
////            Node **ptr_list = find(tree->root, rand());
////            for (int i = 0; ptr_list[i] != NULL; i++)
////                printf("Key: %d | Info: %s\n", ptr_list[i]->key, ptr_list[i]->info);
//        }
//        time_t end = clock();
//        printf("%lf\n", (double) (end - start) / CLOCKS_PER_SEC);
//        deleteTree(tree);
//    }
    printf("in order:\n");
    for (int i = 50000; i <= 1000000; i+= 50000)
    {
        //printf("i = %d\n", i);

        for (int j = 0; j < i; j++)
            insert(tree, makeNode(rand(), ""));
        time_t start = clock();
        Node **ptr_list = NULL; int list_len = 0;
        //ptr_list = preOrderTraversion(tree->root, ptr_list, &list_len);
        Node *ptr = findFirstMin(tree->root);
        while (ptr != NULL)
        {
            //ptr_list = realloc(ptr_list, (list_len + 1) * sizeof (Node *));
            ptr = ptr->next;
            //list_len++;
        }
        time_t end = clock();
        free(ptr_list);
        printf("%lf\n", (double) (end - start) / (double) CLOCKS_PER_SEC);
        //printf("%ld\n", end - start);
        deleteTree(tree);
    }
    printf("Done\n");
    getchar();
    return 0;
}

int D_Random(Tree *tree)
{
    if (tree == NULL)
        return -1;
    deleteTree(tree);
    printf("Enter number of nodes: ");
    unsigned num;
    if (getUINT(&num) < 0)
        return 1;
    srand(time(NULL));
    time_t start = clock();
    for (int n = 0; n < num; n++)
        insert(tree, makeNode(rand(), ""));
    time_t end = clock();
    printf("\n");
    printf("time: %lf\n", (double) (end - start) / CLOCKS_PER_SEC);
    //printf("   Thread traversal: ");
    int i = 0;
    Node **list = NULL;
    for (Node *ptr = findFirstMin(tree->root); ptr != NULL; ptr = ptr->next)
    {
        //printf("%d ", ptr->key);
        list = realloc(list, (i + 1) * sizeof (Node *));
        list[i] = ptr;
        i++;
    }
    //printf("\n");
    //printf("i = %d\n", i);
    int list_len = 0;
    Node **ptr_list = NULL;
//    ptr_list = preOrderTraversal(tree->root, ptr_list, &list_len);
//    printf("Recursive traversal: ");
//    for (int i = 0; i < list_len; i++)
//    {
//        printf("%d ", ptr_list[i]->key);
//    }
//    printf("\n");
    if (i == list_len)
    {
        printf("len is equal\n");
    }
//    else
//    {
//        for (int j = 0; j < list_len; j++)
//        {
//            int found = 0;
//            for (int n = 0; n < i; n++)
//            {
//                if (ptr_list[j] == list[n])
//                {
//                    found = 1;
//                    break;
//                }
//            }
//            if (found == 0)
//                printf("missing: %d\n", ptr_list[j]->key);
//        }
//    }
    //int eq = 1;
    //for (int i = 0; i < list_len; i++)
    //{
    //    if (ptr_list[i] != list[i])
    //    {
    //        printf("Not equal!\n");
    //        eq = 0;
    //        break;
    //    }
    //}
    //if (eq == 0)
    //    printf("eq = 0\n");
    //printTree(tree->root, 0);
    free(list);
    scanf("%*c");
    getchar();
    return 0;
}
