#include <stdio.h>
#include "mylib.h"

int main()
{
    const char *msgs[] = {"0. Quit", "1. Open", "2. Add", "3. Find", "4. Delete", "5. Show"};
    const int NMsgs = sizeof (msgs) / sizeof (msgs[0]);
    FILE *file = NULL;
    int (*fptr[])(Table *, FILE *) = {NULL, NULL, D_Add, D_Find, D_Delete, D_Show};
    Table table;
    resetTable(&table);
    int rc;
    while ((rc = dialog(msgs, NMsgs)) != 0)
    {
        if (rc == 1)
        {
            file = D_Open(&table);
            continue;
        }
        if (fptr[rc](&table, file) == 0)
            break;
    }
    saveTable(&table, file);
    if (file != NULL)
        fclose(file);
    printf("Closed...\n");
    return 0;
}
