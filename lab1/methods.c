#include <stdio.h>
#include "mylib.h"
#include <stdlib.h>
#define swap(t, x, y) { t temp = x; x = y; y = temp;}
#define INVALID printf("Invalid value. Try again...\n")
#define CLOSED printf("Closed...\n")

void erase(Matrix *matrix)
{
    if (matrix != NULL)
    {
        for (int i = 0; i < matrix->lines_num; i++)
        {
            if (matrix->lines[i].line != NULL)
                free(matrix->lines[i].line);
        }
        if (matrix->lines != NULL)
            free(matrix->lines);
        free(matrix);
    }
}

int findIJ(const long *line, int num, int *a, int *b)
{
    *a = -1;
    *b = -1;
    for (int i = 0; i < num - 1; i++)
    {
        if (line[i + 1] > line[i])
        {
            *a = i + 1;
            break;
        }
    }
    for (int j = num - 1; j > 0; j--)
    {
        if (line[j] < line[j - 1])
        {
            *b = j;
            break;
        }
    }
    if (*a == -1 || *b == -1)
        return 0;
    return 1;
}

Matrix *solve(const Matrix *src_matrix)
{
    if (src_matrix == NULL)
        return NULL;
    Matrix *res_matrix = malloc(sizeof (Matrix));
    res_matrix->lines_num = src_matrix->lines_num;
    res_matrix->lines = calloc(src_matrix->lines_num, sizeof (Line));
    for (int i = 0; i < res_matrix->lines_num; i++)
    {
        res_matrix->lines[i].el_num = src_matrix->lines[i].el_num;
        res_matrix->lines[i].line = calloc(res_matrix->lines[i].el_num, sizeof (long));
        for (int j = 0; j < res_matrix->lines[i].el_num; j++)
        {
            res_matrix->lines[i].line[j] = src_matrix->lines[i].line[j];
        }
    }
    for (int i = 0; i < res_matrix->lines_num; i++)
    {
        int a, b;
        if (findIJ(res_matrix->lines[i].line, res_matrix->lines[i].el_num, &a, &b))
            swap(long, res_matrix->lines[i].line[a], res_matrix->lines[i].line[b]);
    }
    return res_matrix;
}

Matrix *input()
{
    Matrix *matrix = malloc(sizeof (Matrix));
    matrix->lines_num = 1;
    matrix->lines = calloc(1, sizeof (Line));
    int flag;
    long temp;
    do
    {
        system("cls");
        printf("Enter number of lines (m): ");
        flag = scanf("%ld", &temp);
        if (flag < 0)
        {
            erase(matrix);
            CLOSED;
            return NULL;
        }
        if (flag == 0 || temp == 0)
        {
            INVALID;
            scanf("%*c");
            flag = 0;
            getchar();
        }
    } while (flag < 1);
    matrix->lines_num = temp;
    matrix->lines = realloc(matrix->lines, matrix->lines_num * sizeof (Line));
    int i = 0, j = 0;
    while (i < matrix->lines_num)
    {
        int flag;
        long temp;
        do
        {
            printf("Enter number of elements in line %d (n%d): ", i + 1, i + 1);
            flag = scanf("%ld", &temp);
            if (flag < 0)
            {
                erase(matrix);
                CLOSED;
                return NULL;
            }
            if (flag == 0 || temp == 0)
            {
                INVALID;
                scanf("%*c");
                flag = 0;
            }
        } while (flag < 1);
        matrix->lines[i].el_num = temp;
        matrix->lines[i].line = calloc(matrix->lines[i].el_num, sizeof (long));
        printf("> ");
        while (j < matrix->lines[i].el_num)
        {
            int flag;
            long temp;
            flag = scanf("%ld", &temp);
            if (flag < 0)
            {
                erase(matrix);
                CLOSED;
                return NULL;
            }
            if (flag == 0)
            {
                INVALID;
                scanf("%*c");
            }
            else
            {
                matrix->lines[i].line[j] = temp;
                j++;
            }
        }
        j = 0;
        i++;
    }
    return matrix;
}

int digits(long input)
{
    int digits = 0;
    if (input == 0)
        return 1;
    while (labs(input) > 0)
    {
        input /= 10;
        digits++;
    }
    return digits;
}

long findMax(const Matrix *matrix)
{
    long max_value = 0;
    for (int i = 0; i < matrix->lines_num; i++)
    {
        for (int j = 0; j < matrix->lines[i].el_num; j++)
        {
            if (labs(matrix->lines[i].line[j]) > labs(max_value))
                max_value = matrix->lines[i].line[j];
        }
    }
    return max_value;
}

int findBelowZero(const Matrix *matrix)
{
    for (int i = 0; i < matrix->lines_num; i++)
    {
        for (int j = 0; j < matrix->lines[i].el_num; j++)
        {
            if (matrix->lines[i].line[j] < 0)
                return -1;
        }
    }
    return 1;
}

void printMatrix(const Matrix *src_matrix, const Matrix *res_matrix)
{
    if (src_matrix == NULL)
    {
        printf("Source matrix is empty\n");
        printf("Press enter to continue . . .");
        getchar();
        return;
    }
    int max_value = findMax(src_matrix);
    printf("\n");
    printf("Source matrix:\n\n");
    for (int i = 0; i < src_matrix->lines_num; i++)
    {
        for (int j = 0; j < src_matrix->lines[i].el_num; j++)
        {
            if (digits(src_matrix->lines[i].line[j]) < digits(max_value))
            {
                for (int x = 0; x < digits(max_value) - digits(src_matrix->lines[i].line[j]); x++)
                    printf(" ");
            }
            if (findBelowZero(src_matrix) < 0 && src_matrix->lines[i].line[j] >= 0)
                printf(" ");
            printf("%ld ", src_matrix->lines[i].line[j]);
        }
        printf("\n\n");
    }
    if (res_matrix == NULL)
    {
        printf("Press enter to continue . . .");
        getchar();
        return;
    }
    max_value = findMax(res_matrix);
    printf("\n");
    printf("Result matrix:\n\n");
    for (int i = 0; i < res_matrix->lines_num; i++)
    {
        for (int j = 0; j < res_matrix->lines[i].el_num; j++)
        {
            if (digits(res_matrix->lines[i].line[j]) < digits(max_value))
            {
                for (int x = 0; x < digits(max_value) - digits(res_matrix->lines[i].line[j]); x++)
                    printf(" ");
            }
            if (findBelowZero(res_matrix) < 0 && res_matrix->lines[i].line[j] >= 0)
                printf(" ");
            printf("%ld ", res_matrix->lines[i].line[j]);
        }
        printf("\n\n");
    }
    printf("Press enter to continue . . .");
    scanf("%*c");
    getchar();
    return;
}
