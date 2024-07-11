#ifndef DIALOG_H
#define DIALOG_H

#include "graph.h"

int dialog(const char *msgs[], const int NMsgs);
int D_AddNode(Graph *graph);
int D_AddEdge(Graph *graph);
int D_DeleteNode(Graph *graph);
int D_DeleteEdge(Graph *graph);
int D_Show(Graph *graph);
int D_FindPathDFS(Graph *graph);
int D_FindMinPath(Graph *graph);
int D_Spec(Graph *graph);
int D_saveToPng(Graph *graph);
int D_saveToTxt(Graph *graph);
int D_loadFromTxt(Graph *graph);
int D_Random(Graph *graph);

#endif // DIALOG_H
