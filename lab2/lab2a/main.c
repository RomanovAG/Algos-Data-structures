#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int main()
{
    while (1)
    {
        Stack stack;
        initStack(&stack);
        char prefix[81];
        int flag;
        do
        {
            printf("Enter the expression in prefix form: ");
            flag = scanf("%80s", prefix);
            if (flag < 0)
            {
                printf("Closed...\n");
                return 0;
            }
            if (flag == 0)
            {
                printf("Wrong value. Try again\n");
                scanf("%*c");
            }
        } while (flag < 1);
        char *result = solve(prefix, &stack);
        if (result != NULL)
            printf("result: %s\n\n", result);
        free(result);
    }
}
