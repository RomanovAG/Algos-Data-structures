#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "D:/labs/AISD/lab4/lab4a/tree.h"
#include <time.h>

char *getStr();

int main()
{
    srand(time(NULL));
    FILE *file = fopen("to_count.bin", "wb");
    int num = 1000;
    for (int i = 0; i < num; i++)
    {
        unsigned k = rand() % num;
        fwrite(&k, sizeof (unsigned), 1, file);
    }
    fclose(file);
    while (1)
    {
        system("cls");
        printf("Enter file name to count words: ");
        char *file_name = getStr();
        if (file_name == NULL)
            return 0;
        FILE *file;
        if ((file = fopen(file_name, "rb")) == NULL)
        {
            perror("Error occured while opening file");
            free(file_name);
            getchar();
            continue;
        }
        Tree tree = {NULL};
        unsigned number;
        while (fread(&number, sizeof (unsigned), 1, file) > 0)
            insert(&tree, makeNode(number, ""));
        fclose(file);
        char *file_out_name = malloc(strlen(file_name) + 11 + 1);
        strcpy(file_out_name, file_name);
        strcat(file_out_name, "_result.txt");
        FILE *file_out = fopen(file_out_name, "w");
        free(file_name);
        free(file_out_name);
        Node *node = findFirstMin(tree.root);
        while (1)
        {
            Node **nodes = find(tree.root, node->key);
            int i = 0;
            while (nodes[i] != NULL)
                i++;
            fprintf(file_out, "%d: %d times\n", node->key, i);
            for (int j = 0; j < i; j++)
                node = findNextInOrder(node);
            if (node == NULL)
                break;
        }
        fclose(file_out);
        deleteTree(&tree);
    }
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
