#ifndef MYLIB_H
#define MYLIB_H
#define START 65536
#define SIZE 32

#include <stdio.h>

typedef struct KeySpace1
{
    int busy;
    char *key;
    long offset;
    int size;
} KeySpace1;

typedef struct KeySpace2
{
    int busy;
    char *key;
    long offset;
    int size;
    int release;
} KeySpace2;

typedef struct Table
{
    KeySpace1 ks1[SIZE];
    KeySpace2 ks2[SIZE];
    int msize1;
    int msize2;
    int csize1;
    int csize2;
} Table;

int getInt(int *integer);
char *getStr();
void resetTable(Table *table);
int insert(Table *table, const char *key1, const char *key2, FILE *file, const char *info);
int delete(Table *table, const char *key1, const char *key2, FILE *file);
int loadTable(Table *table, FILE *file);
int saveTable(Table *table, FILE *file);
int printTable(const Table *table, FILE *file);
Table *newTableKs1(const Table *table, const char *key_start, const char *key_end);
Table *newTableKs2(const Table *table, const char *key, int release);
int dialog(const char *msgs[], const int NMsgs);
FILE *D_Open(Table *table);
int D_Add(Table *table, FILE *file);
int D_Find(Table *table, FILE *file);
int D_Delete(Table *table, FILE *file);
int D_Show(Table *table, FILE *file);

#endif // MYLIB_H
