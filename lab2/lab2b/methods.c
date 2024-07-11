#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

void initStack(Stack *stack)
{
    static int uses = 0;
    if (uses == 0)
        stack->top = NULL;
    if (stack->top == NULL)
        return;
    while (stack->top != NULL)
    {
        free(stack->top->data);
        Item *temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
}

void push(Stack *stack, char *string)
{
    Item *new_item = malloc(sizeof (Item));
    new_item->data = string;
    new_item->next = stack->top;
    stack->top = new_item;
}

char *pop(Stack *stack)
{
    if (stack->top != NULL && stack->top->data != NULL)
    {
        char *string = malloc(strlen(stack->top->data) + 1);
        strcpy(string, stack->top->data);
        free(stack->top->data);
        Item *temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
        return string;
    }
    return NULL;
}

void printStack(Stack *stack)
{
    while (stack->top != NULL)
    {
        printf("%s", stack->top->data);
        stack->top = stack->top->next;
    }
    printf("\n");
    printf("Press enter to continue . . .");
    scanf("%*c");
    getchar();
}

char *toString(char symbol)
{
    char *string = calloc(2, sizeof (char));
    string[0] = symbol;
    string[1] = '\0';
    return string;
}

char *inBrackets(char *string)
{
    string = realloc(string, strlen(string) + 3);
    memmove(string + 1, string, strlen(string) + 1);
    int new_len = strlen(string);
    string[0] = '('; string[new_len] = ')'; string[new_len + 1] = '\0';
    return string;
}

char *delBrackets(char *string)
{
    if (string[0] == '(' && string[strlen(string) - 1] == ')')
    {
        memmove(string, string + 1, strlen(string));
        string[strlen(string) - 1] = '\0';
        return string;
    }
    return string;
}

char *solve(char *prefix, Stack *stack)
{
    for (int i = strlen(prefix) - 1; i >= 0; i--)
    {
        if (strchr("+-*/", prefix[i]) != NULL)
        {
            char *operand_1 = pop(stack);
            char *operand_2 = pop(stack);
            if (operand_1 && operand_2)
            {
                char *operand_new = malloc(strlen(operand_1) + strlen(operand_2) + 2);
                operand_new[0] = '\0';
                operand_new = strcat(operand_new, operand_1);
                free(operand_1);
                char *operator = toString(prefix[i]);
                operand_new = strcat(operand_new, operator);
                free(operator);
                operand_new = strcat(operand_new, operand_2);
                free(operand_2);
                if (strchr("+-", prefix[i]) != NULL)
                {
                    operand_new = inBrackets(operand_new);
                }
                push(stack, operand_new);
            }
            else
            {
                printf("Not a prefix expression...\n\n");
                free(operand_1);
                free(operand_2);
                return NULL;
            }
        }
        else
        {
            push(stack, toString(prefix[i]));
        }
    }
    char *result = pop(stack);
    result = delBrackets(result);
    return result;
}
