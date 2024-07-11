#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

int initStack(Stack *stack)
{
    static int uses = 0;
    if (stack == NULL || sizeof (stack->vector) / sizeof (char *) < 1)
        return 0;
    stack->top = 0;
    stack->size = sizeof (stack->vector) / sizeof (char *);
    for (int i = 0; i < stack->size; i++)
    {
        if (uses > 0)
        {
            free(stack->vector[i]);
        }
        stack->vector[i] = NULL;
    }
    uses++;
    return 1;
}

int push(Stack *stack, char *string)
{
    if (stack->top < stack->size)
    {
        stack->vector[stack->top++] = string;
        return 1;
    }
    return 0;
}

char *pop(Stack *stack)
{
    if (stack->top > 0 && stack->vector[stack->top - 1] != NULL)
    {
        stack->top--;
        char *string = malloc(strlen(stack->vector[stack->top]) + 1);
        strcpy(string, stack->vector[stack->top]);
        free(stack->vector[stack->top]);
        stack->vector[stack->top] = NULL;
        return string;
    }
    return NULL;
}

void printStack(Stack stack)
{
    for (int i = 0 ; i < stack.top; i++)
    {
        printf("%s", stack.vector[i]);
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

/*
int isPrefix(const char *string)
{
    int operand_num = 0;
    int operator_num = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (strchr("+-*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", string[i]) == NULL)
            return 0;
        if (string[i] >= 65 && string[i] <= 90 || string[i] >= 97 && string[i] <= 122 || string[i] >= 48 && string[i] <= 57)
            operand_num++;
        if (strchr("+-*", string[i]) != NULL)
            operator_num++;
    }
    if (operand_num != operator_num + 1 || operator_num == 0)
        return 0;
    if (strchr("+-*", string[strlen(string) - 1]) != NULL || strchr("+-*", string[strlen(string) - 2]) != NULL)
        return 0;
    return 1;
}*/

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
