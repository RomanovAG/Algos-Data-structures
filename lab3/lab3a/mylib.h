#ifndef MYLIB_H
#define MYLIB_H
#define SIZE 64
struct KeySpace1;
struct KeySpace2;

typedef struct Item
{
    char *info;
    char *key1;
    char *key2;
    int ind1;
    int ind2;
    struct KeySpace1 *p1;
    struct KeySpace2 *p2;
} Item;

typedef struct KeySpace1
{
    int busy;
    char *key;
    Item *info;
} KeySpace1;

typedef struct KeySpace2
{
    int busy;
    char *key;
    int release;
    Item *info;
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


char *getStr();
int getInt(int *integer);
void resetTable(Table *table);
int dialog(const char *msgs[], const int NMsgs);
int insert(Table *table, const char *key1, const char *key2, const char *data);
int delete(Table *table, const char *key1, const char *key2);
void delTable(Table *table);
int printTable(const Table *table);
Table *newTableKs1(const Table *table, const char *key_start, const char *key_end);
Table *newTableKs2(const Table *table, const char *key, int release);
int D_Add(Table *table);
int D_Find(Table *table);
int D_Delete(Table *table);
int D_Show(Table *table);

#endif // MYLIB_H
