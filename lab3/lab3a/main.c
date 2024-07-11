#include <stdio.h>
#include "mylib.h"

int main()
{
    const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
    const int NMsgs = sizeof (msgs) / sizeof (msgs[0]);
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};
    Table table;
    resetTable(&table);
    int rc;
    while ((rc = dialog(msgs, NMsgs)) != 0)
    {
        if (fptr[rc](&table) == 0)
            break;
    }
    delTable(&table);
    printf("Closed...\n");
    return 0;
}
