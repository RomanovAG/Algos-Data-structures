#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mylib.h"
int strToInt(const char *string);
void resetTable(Table *table)
{
    table->csize1 = 0;
    table->csize2 = 0;
    table->msize1 = sizeof (table->ks1) / sizeof (table->ks1[0]);
    table->msize2 = sizeof (table->ks2) / sizeof (table->ks2[0]);
    for (int i = 0; i < table->msize1; i++)
    {
        table->ks1[i].busy = 0;
        table->ks1[i].info = NULL;
        table->ks1[i].key = NULL;
    }
    for (int i = 0; i < table->msize2; i++)
    {
        table->ks2[i].busy = 0;
        table->ks2[i].info = NULL;
        table->ks2[i].key = NULL;
        table->ks2[i].release = 0;
    }
}

int h1(const char *key, int size)
{
    return strToInt(key) + key[strToInt(key) % strlen(key)] % size;
}

int h2(const char *key, int size)
{
    int temp = strToInt(key) + key[strToInt(key) % strlen(key)] % size;
    if (temp % 2 == 0)
        temp++;
    return temp;
}

int hash2(const char *key, int i, int size)
{
    return (h1(key, size) + i * h2(key, size)) % size;
}

int strToInt(const char *string)
{
    if (string == NULL)
        return 0;
    int result = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        result += string[i];
    }
    return result;
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

void delTable(Table *table)
{
    table->csize1 = 0;
    table->csize2 = 0;
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy > 0)
        {
            free(table->ks1[i].info->key1);
            free(table->ks1[i].info->key2);
            table->ks1[i].info->p2->busy = 0;
            table->ks1[i].info->p2->release = 0;
            free(table->ks1[i].info);
            table->ks1[i].busy = 0;
        }
    }
}

int getInt(int *integer)
{
    int flag;
    int temp;
    do
    {
        printf(">");
        flag = scanf("%d", &temp);
        if (flag < 0)
            return 0;
        if (flag == 0)
        {
            printf("Wrong value. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while (flag < 1);
    *integer = temp;
    return 1;
}

int *findAllIndKs1(const Table *table, const char *key_start, const char *key_end)
{
    int *arr = calloc(64, sizeof (int));
    for (int i = 0; i < 64; i++)
        arr[i] = -1;
    int index = 0;
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy == 1 && strcmp(table->ks1[i].key, key_start) >= 0 && strcmp(table->ks1[i].key, key_end) <= 0)
        {
            arr[index] = i;
            index++;
        }
    }
    return arr;
}
int findKey2(const Table *table, const char *key2, int release);
int *findAllIndKs2(const Table *table, const char *key, int release)
{
    if (release > 0)
    {
        int *arr = malloc(2 * sizeof (int));
        arr[0] = findKey2(table, key, release);
        arr[1] = -1;
        return arr;
    }
    int *arr = calloc(64, sizeof (int));
    for (int i = 0; i < 64; i++)
        arr[i] = -1;
    int index = 0;
    for (int i = 0; i < table->msize2; i++)
    {
        int j = hash2(key, i, table->msize2);
        if (table->ks2[j].busy == 0)
            return arr;
        if (table->ks2[j].busy == 1 && strcmp(table->ks2[j].key, key) == 0)
        {
            arr[index] = j;
            index++;
        }
    }
    return arr;
}

int findKey1(const Table *table, const char *key1)
{
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy == 1 && strcmp(table->ks1[i].key, key1) == 0)
            return i;
    }
    return -1;
}

int findKey2(const Table *table, const char *key2, int release)
{
    for (int i = 0; i < table->msize2; i++)
    {
        int j = hash2(key2, i, table->msize2);
        if (table->ks2[j].busy == 0)
            return -1;
        if (table->ks2[j].busy == 1 && strcmp(table->ks2[j].key, key2) == 0 && table->ks2[j].release == release)
            return j;
    }
    return -1;
}

int key2Release(Table *table, const char *key2)
{
    int release = 0;
    for (int i = 0; i < table->msize2; i++)
    {
        int j = hash2(key2, i, table->msize2);
        if (table->ks2[j].busy == 0)
            return release + 1;
        if (table->ks2[j].key != NULL && strcmp(table->ks2[j].key, key2) == 0 && table->ks2[j].release > 0)
            release = table->ks2[j].release;
    }
    return release + 1;
}

KeySpace1 makeKey1(Item *info, const char *key1)
{
    KeySpace1 Key;
    Key.busy = 1;
    Key.key = malloc(strlen(key1) + 1);
    strcpy(Key.key, key1);
    Key.info = info;
    return Key;
}

KeySpace2 makeKey2(Item *info, const char *key2, int release)
{
    KeySpace2 Key;
    Key.busy = 1;
    Key.key = malloc(strlen(key2) + 1);
    strcpy(Key.key, key2);
    Key.info = info;
    Key.release = release;
    return Key;
}

int cleanKs1(Table *table)
{
    int j = 0;
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy == 1)
        {
            table->ks1[j] = table->ks1[i];
            table->ks1[j].info->ind1 = j;
            table->ks1[j].info->p1 = &table->ks1[j];
            j++;
        }
    }
    table->csize1 = j;
    return j;
}

int insertKey1(Table *table, KeySpace1 key1)
{
    //printf("index 1: %d\n", table->csize1);
    if (table->csize1 == table->msize1)
    {
        cleanKs1(table);
        if (table->csize1 == table->msize1)
            return 2;
    }
    if (findKey1(table, key1.key) >= 0)
        return 1;
    table->ks1[table->csize1] = key1;
    table->ks1[table->csize1].info->ind1 = table->csize1;
    table->ks1[table->csize1].info->p1 = &table->ks1[table->csize1];
    table->csize1++;
    return 0;
}

int insertKey2(Table *table, KeySpace2 key2)
{
    for (int i = 0; i < table->msize2; i++)
    {
        int j = hash2(key2.key, i, table->msize2);
        if (table->ks2[j].busy != 1)
        {
            table->ks2[j] = key2;
            table->ks2[j].info->ind2 = j;
            table->ks2[j].info->p2 = &table->ks2[j];
            printf("index 2: %d\n", j);
            table->csize2++;
            return 0;
        }
    }
    return 2;
}

int insert(Table *table, const char *key1, const char *key2, const char *data)
{
    Item *info = malloc(sizeof (Item));
    info->info = malloc(strlen(data) + 1);
    strcpy(info->info, data);
    info->p1 = NULL, info->p2 = NULL;
    //printf("starting insert...\n");
    KeySpace1 KEY1 = makeKey1(info, key1);
    //printf("key1 made . . .\n");
    int n = insertKey1(table, KEY1);
    if (n > 0)
    {
        free(KEY1.key);
        free(KEY1.info);
        return n;
    }
    KeySpace2 KEY2 = makeKey2(info, key2, key2Release(table, key2));
    //printf("key2.release: %d\n", KEY2.release);
    n = insertKey2(table, KEY2);
    if (n > 0)
    {
        free(KEY2.key);
        free(KEY2.info);
        return n;
    }
    //printf("key2: \"%s\"\n", KEY2.key);
    info->key1 = KEY1.key, info->key2 = KEY2.key;
    return 0;
}

void delKey1(Table *table, int index)
{
    table->ks1[index].busy = 2;
    free(table->ks1[index].key);
    free(table->ks1[index].info->info);
    table->ks1[index].key = NULL;
    table->ks1[index].info->info = NULL;
    table->ks1[index].info->key1 = NULL;
    table->ks1[index].info->key2 = NULL;
    free(table->ks1[index].info);
    table->ks1[index].info = NULL;
}

void delKey2(Table *table, int index)
{
    table->ks2[index].busy = 2;
    table->ks2[index].release = 0;
    free(table->ks2[index].key);
//printf("debug1\n");
    //free(table->ks2[index].info->info);
    table->ks2[index].key = NULL;
    //table->ks2[index].info->info = NULL;
    //table->ks2[index].info->key1 = NULL;
    //table->ks2[index].info->key2 = NULL;
    //free(table->ks2[index].info);
    table->ks2[index].info = NULL;
    //printf("debug2\n");
}

int delete(Table *table, const char *key1, const char *key2)
{
    if (key1 == NULL && key2 == NULL)
        return 1;
    //printf("Entering delete...\n");
    if (key1 != NULL && key2 != NULL)
    {
        int index1 = findKey1(table, key1);
        printf("index1: %d\n", index1);
        //printf("key2: %s\n", table->ks1[index1].info->p2->key);
        //printf("key2: %s\n", key2);
        if (strcmp(table->ks1[index1].info->p2->key, key2) != 0)
            return 2;
        int index2 = table->ks1[index1].info->ind2;
        //printf("index2: %d\n", index2);
        delKey1(table, index1);
        delKey2(table, index2);
    }
    if (key1 != NULL && key2 == NULL)
    {
        int index1 = findKey1(table, key1);
        //printf("index1: %d\n", index1);
        int index2 = table->ks1[index1].info->ind2;
        //printf("index2: %d\n", index2);
        delKey1(table, index1);
        delKey2(table, index2);
    }
    if (key1 == NULL && key2 != NULL)
    {
        //printf("key2 != NULL\n");
        int index2;
        int i = 1;
        //printf("index2: %d\n", findKey2(table, key2, i));
        while ((index2 = findKey2(table, key2, i)) >= 0)
        {
            //printf("index2: %d\n", index2);
            int index1 = table->ks2[index2].info->ind1;
            delKey1(table, index1);
            delKey2(table, index2);
            //printf("index1: %d\n", index1);
            i++;
        }
    }
    return 0;
}

int printTable(const Table *table)
{
    int is_empty = 1;
    for (int i = 0; i < table->msize1; i++)
    {
        //printf("busy: %d\n", table->ks1[i].busy);
        if (table->ks1[i].busy == 1)
        {
            is_empty = 0;
            printf("Key 1: \"%s\" | Key 2: \"%s\" (release %d) | Info: \"%s\"\n",
                   table->ks1[i].key,
                   table->ks1[i].info->key2,
                   table->ks1[i].info->p2->release,
                   table->ks1[i].info->info);
        }
    }
    if (is_empty == 1)
        return 0;
    return 1;
}

Table *newTableKs1(const Table *table, const char *key_start, const char *key_end)
{
    int *arr = findAllIndKs1(table, key_start, key_end);
    int msize = 0;
    while (arr[msize] != -1)
        msize++;
    Table *new_t = malloc(sizeof (Table));
    new_t->msize1 = msize;
    new_t->msize2 = msize;
    for (int i = 0; i < msize; i++)
    {
        new_t->ks1[i] = table->ks1[arr[i]];
    }
    free(arr);
    return new_t;
}

Table *newTableKs2(const Table *table, const char *key, int release)
{
    int *arr = findAllIndKs2(table, key, release);
    int msize = 0;
    while (arr[msize] != -1)
        msize++;
    Table *new_t = malloc(sizeof (Table));
    new_t->msize1 = msize;
    new_t->msize2 = msize;
    for (int i = 0; i < msize; i++)
    {
        new_t->ks1[i] = *table->ks2[arr[i]].info->p1;
    }
    free(arr);
    return new_t;
}
