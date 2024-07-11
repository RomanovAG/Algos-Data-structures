#include "graph.h"
#include "dialog.h"
#include <stdio.h>

int main()
<%
    Graph graph = {0, NULL};
    const char *msgs[] = <%"0. Quit", "1. Add node", "2. Add edge", "3. Delete node", "4. Delete edge",
                          "5. Show", "6. Find path", "7. Find minimum path", "8. Special operation",
                          "9. Save to PNG", "10. Save to TXT", "11. Load from TXT"%>;
    const int NMsgs = sizeof (msgs) / sizeof (msgs[0]);
    int (*fptr[])(Graph *) = <%NULL, D_AddNode, D_AddEdge, D_DeleteNode, D_DeleteEdge, D_Show, D_FindPathDFS, D_FindMinPath, D_Spec,
                              D_saveToPng, D_saveToTxt, D_loadFromTxt%>;
    int choice;
    while ((choice = dialog(msgs, NMsgs)) > 0)
        if (fptr[choice](&graph) < 0)
            break;
    deleteGraph(&graph);
    printf("Closed...\n");
    return 0;
%>
