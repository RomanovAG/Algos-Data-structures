#include <stdio.h>
#include "b-tree.h"
#include <stdlib.h>
#include <string.h>

char *getStr();
char *fgetStr(FILE *file);

int main()
{
    FILE *file;
    while (1)
    {
        system("cls");
        printf("Enter file name to find words: ");
        char *file_name = getStr();
        if (file_name == NULL)
            return 0;
        if ((file = fopen(file_name, "rb")) == NULL)
        {
            perror("Error occured while opening file");
            free(file_name);
            getchar();
            continue;
        }
        free(file_name);
        break;
    }
    B_Tree tree = {makeEmptyNode()};
    long line = 1;
    char *string;
    while ((string = fgetStr(file)) != NULL)
    {
        string[strlen(string) - 1] = '\0';
        //printf("strlen %d\n", strlen(string));
        char *word = strtok(string, " ");
        long offset = 1;
        while (word != NULL)
        {
            Offset info;
            info.line = line;
            info.offset = offset;
            offset += strlen(word) + 1;
            if (strchr(".,!?;:", word[strlen(word) - 1]) != NULL)
                word[strlen(word) - 1] = '\0';
            bInsert(&tree.root, word, info);
            word = strtok(NULL, " ");
        }
        line++;
        free(string);
    }
    //printf("\n");
    //printTree(tree.root, 0);
    fclose(file);
    //getchar();
    while (1)
    {
        system("cls");
        printf("Enter word to find: ");
        char *word = getStr();
        if (word == NULL)
        {
            bDeleteTree(&tree.root);
            free(tree.root);
            return 0;
        }
        Key_pos *kp = bSearch(tree.root, word);
        if (kp == NULL)
            printf("No word found\n");
        else
            printf("Line: %ld | Offset: %ld\n", kp->ptr->info[kp->index]->line, kp->ptr->info[kp->index]->offset);
        free(word);
        getchar();
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
