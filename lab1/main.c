#include "mylib.h"

int main()
{
    while (1)
    {
        Matrix *new_matrix = input();
        if (new_matrix == (void *)0)
            return 0;
        Matrix *res = solve(new_matrix);
        printMatrix(new_matrix, res);
        erase(new_matrix);
        erase(res);
    }
}
