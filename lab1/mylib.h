#ifndef MYLIB_H
#define MYLIB_H

typedef struct Line
{
    int el_num;
    long *line;
} Line;

typedef struct Matrix
{
    int lines_num;
    Line *lines;
} Matrix;

void erase(Matrix *matrix);
Matrix *solve(const Matrix *src_matrix);
Matrix *input();
void printMatrix(const Matrix *src_matrix, const Matrix *res_matrix);

#endif // MYLIB_H
