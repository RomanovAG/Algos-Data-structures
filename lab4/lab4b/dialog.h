#ifndef DIALOG_H
#define DIALOG_H

#include "b-tree.h"
#include <stdlib.h>

int dialog(const char *msgs[], const int NMsgs);
int D_Add(B_Tree *tree);
int D_Delete(B_Tree *tree);
int D_Traversal(B_Tree *tree);
int D_Find(B_Tree *tree);
int D_Show(B_Tree *tree);
int D_saveToPng(B_Tree *tree);
int D_saveToTxt(B_Tree *tree);
int D_loadFromTxt(B_Tree *tree);
int D_Timing(B_Tree *tree);
int D_Random(B_Tree *tree);
char *increment(char *str);

#endif // DIALOG_H
