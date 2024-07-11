#include <stdio.h>
#include "tree.h"
#include "dialog.h"

int main()
{
    Tree tree = {NULL};
    const char *msgs[] = {"0. Quit", "1. Add", "2. Delete", "3. Traversal", "4. Find", "5. Show",
                          "6. Save to PNG", "7. Save to TXT", "8. Load from TXT", "9. Timing", "10. Make random tree"};
    const int NMsgs = sizeof (msgs) / sizeof (msgs[0]);
    int (*fptr[])(Tree *) = {NULL, D_Add, D_Delete, D_Traversal, D_Find, D_Show,
                             D_saveToPng, D_saveToTxt, D_loadFromTxt, D_Timing, D_Random};
    int choice;
    while ((choice = dialog(msgs, NMsgs)) > 0)
        if (fptr[choice](&tree) < 0)
            break;
    deleteTree(&tree);
    printf("Closed...\n");
    return 0;
}
