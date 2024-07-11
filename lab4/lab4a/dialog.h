#ifndef DIALOG_H
#define DIALOG_H

#include "tree.h"

int dialog(const char *msgs[], const int NMsgs);
int D_Add(Tree *tree);
int D_Delete(Tree *tree);
int D_Traversal(Tree *tree);
int D_Find(Tree *tree);
int D_Show(Tree *tree);
int D_saveToPng(Tree *tree);
int D_saveToTxt(Tree *tree);
int D_loadFromTxt(Tree *tree);
int D_Timing(Tree *tree);
int D_Random(Tree *tree);
char *increment(char *str);

#endif // DIALOG_H
