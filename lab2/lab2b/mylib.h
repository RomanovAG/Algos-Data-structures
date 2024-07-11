#ifndef MYLIB_H
#define MYLIB_H

typedef struct Item
{
    char *data;
    struct Item *next;
} Item;

typedef struct Stack
{
    Item *top;
} Stack;

void initStack(Stack *stack);
void push(Stack *stack, char *string);
char *pop(Stack *stack);
void printStack(Stack *stack);
char *solve(char *prefix, Stack *stack);

#endif // MYLIB_H
