#ifndef MYLIB_H
#define MYLIB_H

#define SIZE 32

typedef struct Stack
{
    char *vector[SIZE];
    int size;
    int top;
} Stack;

int initStack(Stack *stack);
int push(Stack *stack, char *string);
char *pop(Stack *stack);
void printStack(Stack stack);
char *solve(char *prefix, Stack *stack);

#endif // MYLIB_H
