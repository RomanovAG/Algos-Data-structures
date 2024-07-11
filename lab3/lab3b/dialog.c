#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mylib.h"

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

FILE *D_Open(Table *table)
{
    printf("Enter file name: ");
    char *file_name = getStr();
    if (file_name == NULL)
        return NULL;
    FILE *file = fopen(file_name, "r+b");
    if (file == NULL)
    {
        file = fopen(file_name, "w+b");
        fseek(file, START, SEEK_SET);
        fputc('#', file);
        free(file_name);
        return file;
    }
    loadTable(table, file);
    free(file_name);
    return file;
}

int D_Add(Table *table, FILE *file)
{
    if (file == NULL)
    {
        printf("No file opened...\n");
        getchar();
        return 1;
    }
    char *msgs[] = {"Success", "Keys are equal...", "Table is full..."};
    printf("Enter key 1: ");
    char *key1 = getStr();
    if (key1 == NULL)
        return 0;
    printf("Enter key 2: ");
    char *key2 = getStr();
    if (key2 == NULL)
    {
        free(key1);
        return 0;
    }
    printf("Enter data: ");
    char *data = NULL;
    data = getStr();
    if (data == NULL)
    {
        free(key1), free(key2);
        return 0;
    }
    int rc = insert(table, key1, key2, file, data);
    printf("%s\n", msgs[rc]);
    getchar();
    return 1;
}

int D_Find(Table *table, FILE *file)
{
    int choice;
    printf("Enter key space\n");
    if (getInt(&choice) == 0)
        return 0;
    if (choice == 1)
    {
        printf("Enter two keys to set the range\n");
        printf("Enter first key: ");
        scanf("%*c");
        char *key_start = getStr();
        if (key_start == NULL)
            return 0;
        printf("Enter second key: ");
        char *key_end = getStr();
        if (key_end == NULL)
        {
            free(key_start);
            return 0;
        }
        Table *new_table = newTableKs1(table, key_start, key_end);
        printTable(new_table, file);
        free(new_table);
    }
    if (choice == 2)
    {
        printf("Enter key from key space 2: ");
        scanf("%*c");
        char *key = getStr();
        if (key == NULL)
            return 0;
        printf("Enter release (type \"0\" to ignore it)\n");
        int release = 0;
        if (getInt(&release) == 0)
            return 0;
        Table *new_table = newTableKs2(table, key, release);
        printTable(new_table, file);
        free(new_table);
        scanf("%*c");
    }
    getchar();
    return 1;
}

int D_Delete(Table *table, FILE *file)
{
    char *msgs[] = {"Success", NULL, "Keys not found..."};
    printf("1. Delete by 2 keys\n"
           "2. Delete by key\n");
    int choice;
    if (getInt(&choice) == 0)
        return 0;
    if (choice == 1)
    {
        printf("Enter key 1: ");
        scanf("%*c");
        char *key1 = getStr();
        if (key1 == NULL)
            return 0;
        printf("Enter key 2: ");
        //scanf("%*c");
        char *key2 = getStr();
        if (key2 == NULL)
        {
            free(key1);
            return 0;
        }
        int rc = delete(table, key1, key2, file);
        printf("%s", msgs[rc]);
        free(key1), free(key2);
    }
    else if (choice == 2)
    {
        printf("Enter key space\n");
        int key_space;
        if (getInt(&key_space) == 0)
            return 0;
        scanf("%*c");
        printf("Enter key from key space %d: ", key_space);
        char *key = getStr();
        if (key == NULL)
            return 0;
        if (key_space == 1)
            delete(table, key, NULL, file);
        else if (key_space == 2)
            delete(table, NULL, key, file);
        free(key);
    }
    getchar();
    return 1;
}

int D_Show(Table *table, FILE *file)
{
    if (printTable(table, file) == 0)
        printf("Table is empty...");
    getchar();
    return 1;
}
