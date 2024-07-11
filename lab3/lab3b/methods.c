 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mylib.h"


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

void resetTable(Table *table)
{
    table->csize1 = 0;
    table->csize2 = 0;
    table->msize1 = sizeof (table->ks1) / sizeof (table->ks1[0]);
    table->msize2 = sizeof (table->ks2) / sizeof (table->ks2[0]);
    for (int i = 0; i < table->msize1; i++)
    {
        table->ks1[i].busy = 0;
        table->ks1[i].key = NULL;
        table->ks1[i].offset = -1;
        table->ks1[i].size = 0;
    }
    for (int i = 0; i < table->msize2; i++)
    {
        table->ks2[i].busy = 0;
        table->ks2[i].key = NULL;
        table->ks2[i].release = 0;
        table->ks2[i].offset = -1;
        table->ks2[i].size = 0;
    }
}

int strToInt(const char *string);

int h1(const char *key, int size)
{
    if (strlen(key) == 0)
        return -1;
    return strToInt(key) + key[10 % strlen(key)] % size;
}

int h2(const char *key, int size)
{
    if (strlen(key) == 0)
        return -1;
    int temp = strToInt(key) + key[10 % strlen(key)] % size;
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

int findByOffset(const Table *table, int key_space, long offset)
{
    if (key_space == 1)
    {
        for (int i = 0; i < table->msize1; i++)
        {
            if (table->ks1[i].offset == offset)
                return i;
        }
    }
    if (key_space == 2)
    {
        for (int j = 0; j < table->msize2; j++)
        {
            if (table->ks2[j].offset == offset)
            {
                //printf("found! %d\n", j);
                return j;
            }
        }
    }
    return -1;
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
        if (table->ks2[j].busy == 1 && strcmp(table->ks2[j].key, key2) == 0 && (table->ks2[j].release == release))
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

KeySpace1 makeKey1(const char *key1, const char *info, FILE *file)
{
    KeySpace1 Key;
    Key.busy = 1;
    Key.key = malloc(strlen(key1) + 1);
    Key.key = strcpy(Key.key, key1);
    //long current_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    Key.offset = ftell(file);
    //fseek(file, current_pos, SEEK_SET);
    Key.size = strlen(info);
    return Key;
}

KeySpace2 makeKey2(const char *key2, const char *info, int release, KeySpace1 Key1)
{
    KeySpace2 Key;
    Key.busy = 1;
    Key.key = malloc(strlen(key2) + 1);
    Key.key = strcpy(Key.key, key2);
    Key.release = release;
    Key.offset = Key1.offset;
    Key.size = strlen(info);
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
            //printf("index 2: %d\n", j);
            table->ks2[j] = key2;
            table->csize2++;
            return 0;
        }
    }
    return 2;
}

int toFile(const char *info, FILE *file)
{
    fseek(file, 0, SEEK_END);
    fwrite(info, sizeof (char), strlen(info), file);
    fseek(file, 0, SEEK_END);
    return 0;
}

int insert(Table *table, const char *key1, const char *key2, FILE *file, const char *info)
{
    //printf("starting insert...\n");
    KeySpace1 KEY1 = makeKey1(key1, info, file);
    //printf("key1 made . . .\n");
    int n = insertKey1(table, KEY1);
    if (n > 0)
    {
        free(KEY1.key);
        return n;
    }
    KeySpace2 KEY2 = makeKey2(key2, info, key2Release(table, key2), KEY1);
    //printf("key2.release: %d\n", KEY2.release);
    n = insertKey2(table, KEY2);
    if (n > 0)
    {
        free(KEY2.key);
        return n;
    }
    toFile(info, file);
    //printf("key2: \"%s\"\n", KEY2.key);
    return 0;
}

void delKey1(Table *table, int index)
{
    table->ks1[index].busy = 2;
    free(table->ks1[index].key);
    table->ks1[index].key = NULL;
    table->ks1[index].offset = -1;
    table->ks1[index].size = 0;
}

void delKey2(Table *table, int index)
{
    table->ks2[index].busy = 2;
    table->ks2[index].release = 0;
    free(table->ks2[index].key);
    //printf("debug1\n");
    table->ks2[index].key = NULL;
    table->ks2[index].offset = -1;
    table->ks2[index].size = 0;
    //printf("debug2\n");

}

int cleanFile(Table *table, FILE *file)
{
    if (file == NULL)
        return -1;
    int offset = START + 1;
    fseek(file, START, SEEK_SET);
    putc('#', file);
    for (int index1 = 0; index1 < table->msize1; index1++)
    {
        if (table->ks1[index1].busy == 1)
        {
            int index2 = findByOffset(table, 2, table->ks1[index1].offset);
            fseek(file, table->ks1[index1].offset, SEEK_SET);
            int size = table->ks1[index1].size;
            char *info = malloc( + 1);
            fread(info, sizeof (char), size, file);
            info[size] = '\0';
            table->ks1[index1].offset = offset;
            table->ks2[index2].offset = offset;
            fseek(file, table->ks1[index1].offset, SEEK_SET);
            fwrite(info, sizeof (char), size, file);
            offset += size;
        }
    }
    return 0;
}

int delete(Table *table, const char *key1, const char *key2, FILE *file)
{
    if (key1 == NULL && key2 == NULL)
        return 1;
    //printf("Entering delete...\n");
    if (key1 != NULL && key2 != NULL)
    {
        int index1 = findKey1(table, key1);
        if (index1 < 0)
            return 2;
        //printf("index1: %d\n", index1);
        //printf("key2: %s\n", key2);
        int index2 = findByOffset(table, 2, table->ks1[index1].offset);
        if (strcmp(table->ks2[index2].key, key2) != 0)
            return 2;
        //printf("index2: %d\n", index2);
        delKey1(table, index1);
        delKey2(table, index2);
    }
    if (key1 != NULL && key2 == NULL)
    {
        int index1 = findKey1(table, key1);
        //printf("index1: %d\n", index1);
        int index2 = findByOffset(table, 2, table->ks1[index1].offset);
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
            int index1 = findByOffset(table, 1, table->ks2[index2].offset);
            delKey1(table, index1);
            delKey2(table, index2);
            //printf("index1: %d\n", index1);
            i++;
        }
    }
    cleanFile(table, file);
    return 0;
}

int loadTable(Table *table, FILE *file)
{
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        return 1;
    }
    fseek(file, 0, SEEK_SET);
    int csize1, csize2;
    fread(&csize1, sizeof (int), 1, file);
    fread(&csize2, sizeof (int), 1, file);
    //table->csize1 = csize1;
    table->csize2 = csize2;
    for (int i = 0; i < csize1; i++)
    {
        KeySpace1 key1;
        //int index1;
        fread(&key1.busy, sizeof (key1.busy), 1, file);

            int key_size;
            fread(&key_size, sizeof (key_size), 1, file);
            key1.key = malloc(key_size + 1);
            fread(key1.key, sizeof (char), key_size, file);
            key1.key[key_size] = '\0';
            fread(&key1.offset, sizeof (key1.offset), 1, file);
            fread(&key1.size, sizeof (key1.size), 1, file);
            //table->ks1[index1] = key1;
        insertKey1(table, key1);
    }
    for (int j = 0; j < csize2; j++)
    {
        KeySpace2 key2;
        int index2;
        fread(&index2, sizeof (index2), 1, file);
        fread(&key2.busy, sizeof (key2.busy), 1, file);

            int key_size;
            fread(&key_size, sizeof (key_size), 1, file);
            key2.key = malloc(key_size + 1);
            fread(key2.key, sizeof (char), key_size, file);
            key2.key[key_size] = '\0';
            fread(&key2.offset, sizeof (key2.offset), 1, file);
            fread(&key2.size, sizeof (key2.size), 1, file);
            fread(&key2.release, sizeof (key2.release), 1, file);
            table->ks2[index2] = key2;
        //insertKey2(table, key2);
    }
    fseek(file, START, SEEK_SET);
    putc('#', file);
    return 0;
}

int saveTable(Table *table, FILE *file)
{
    if (file == NULL)
        return -1;
    fseek(file, 0, SEEK_SET);
    fwrite(&table->csize1, sizeof (table->csize1), 1, file);
    //long offset = ftell(file);
    //printf("offset %ld\n", offset);
    fwrite(&table->csize2, sizeof (table->csize2), 1, file);
    int csize1 = 0;
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy == 1)
        {
            fwrite(&table->ks1[i].busy, sizeof (table->ks1[i].busy), 1, file);
            int key_size = strlen(table->ks1[i].key);
            fwrite(&key_size, sizeof (key_size), 1, file);
            fwrite(table->ks1[i].key, sizeof (char), key_size, file);
            fwrite(&table->ks1[i].offset, sizeof (table->ks1[i].offset), 1, file);
            fwrite(&table->ks1[i].size, sizeof (table->ks1[i].size), 1, file);
            csize1++;
        }
    }
    int csize2 = 0;
    for (int j = 0; j < table->msize2; j++)
    {
        if (table->ks2[j].busy == 1)
        {
            fwrite(&j, sizeof (j), 1, file);
            fwrite(&table->ks2[j].busy, sizeof (table->ks2[j].busy), 1, file);
            int key_size = strlen(table->ks2[j].key);
            fwrite(&key_size, sizeof (key_size), 1, file);
            fwrite(table->ks2[j].key, sizeof (char), key_size, file);
            fwrite(&table->ks2[j].offset, sizeof (table->ks2[j].offset), 1, file);
            fwrite(&table->ks2[j].size, sizeof (table->ks2[j].size), 1, file);
            fwrite(&table->ks2[j].release, sizeof (table->ks2[j].release), 1, file);
            csize2++;
        }
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&csize1, sizeof (int), 1, file);
    fwrite(&csize2, sizeof (int), 1, file);
    return 0;
}

int printTable(const Table *table, FILE *file)
{
    int is_empty = 1;
    for (int i = 0; i < table->msize1; i++)
    {
        if (table->ks1[i].busy == 1)
        {
            is_empty = 0;
            int ind2 = findByOffset(table, 2, table->ks1[i].offset);
            fseek(file, table->ks1[i].offset, SEEK_SET);
            char *info = malloc(table->ks1[i].size + 1);
            fread(info, sizeof (char), table->ks1[i].size, file);
            info[table->ks1[i].size] = '\0';
            /*
            printf("Key 1: \"%s\" index: \"%d\" | Key 2: \"%s\" (release %d) index: \"%d\" | Info: \"%s\"\n",
                   table->ks1[i].key, i,
                   table->ks2[ind2].key,
                   table->ks2[ind2].release, ind2, info);
*/
            printf("Key 1: \"%s\" | Key 2: \"%s\" (release %d) | Info: \"%s\"\n",
                   table->ks1[i].key,
                   table->ks2[ind2].key,
                   table->ks2[ind2].release, info);
        }
    }
    if (is_empty == 1)
        return 0;
    return 1;
}

int *findAllIndKs1(const Table *table, const char *key_start, const char *key_end)
{
    int *arr = calloc(SIZE, sizeof (int));
    for (int i = 0; i < SIZE; i++)
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

int *findAllIndKs2(const Table *table, const char *key, int release)
{
    if (release > 0)
    {
        int *arr = malloc(2 * sizeof (int));
        arr[0] = findKey2(table, key, release);
        arr[1] = -1;
        return arr;
    }
    int *arr = calloc(SIZE, sizeof (int));
    for (int i = 0; i < SIZE; i++)
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

Table *newTableKs1(const Table *table, const char *key_start, const char *key_end)
{
    int *arr = findAllIndKs1(table, key_start, key_end);
    int csize = 0;
    while (arr[csize] != -1)
        csize++;
    Table *new_t = malloc(sizeof (Table));
    new_t->msize1 = SIZE;
    new_t->msize2 = SIZE;
    new_t->csize1 = csize;
    new_t->csize2 = csize;
    for (int i = 0; i < new_t->msize1; i++)
    {
        new_t->ks1[i] = table->ks1[arr[i]];
        //printf("key1 %s\n", new_t->ks1[i].key);
        int index2 = findByOffset(table, 2, new_t->ks1[i].offset);
        //printf("key2 %s\n", table->ks2[index2].key);
        //printf("index2 %d\n", index2);
        new_t->ks2[index2] = table->ks2[index2];
        //insertKey2(new_t, table->ks2[findByOffset(table, 2, new_t->ks1[i].offset)]);
    }
    free(arr);
    return new_t;
}

Table *newTableKs2(const Table *table, const char *key, int release)
{
    int *arr = findAllIndKs2(table, key, release);
    int csize = 0;
    while (arr[csize] != -1)
        csize++;
    Table *new_t = malloc(sizeof (Table));
    new_t->msize1 = SIZE;
    new_t->msize2 = SIZE;
    new_t->csize1 = csize;
    new_t->csize2 = csize;
    for (int i = 0; arr[i] != -1; i++)
    {
        new_t->ks1[i] = table->ks1[findByOffset(table, 1, table->ks2[arr[i]].offset)];
        //printf("key2 %s\n", table->ks2[arr[i]].key);
        //printf("index2 %d\n", arr[i]);
        insertKey2(new_t, table->ks2[arr[i]]);
    }
    free(arr);
    return new_t;
}
