#ifndef SOFTWAREPROJECT_MATRIX_H
#define SOFTWAREPROJECT_MATRIX_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "errors.h"

typedef struct {
    int rows;
    int cols;
    double *data;
} matrix;

matrix *newMatrix(int rows, int cols);

int deleteMatrix(matrix *mtx);

matrix *copyMatrix(matrix *mtx);

int setElement(matrix *mtx, int row, int col, double val);

double getElement(matrix *mtx, int row, int col);

int nRows(matrix *mtx);

int nCols(matrix *mtx);

int printMatrix(matrix *mtx);

int sub(matrix *mtx1, matrix *mtx2, matrix *sub);

int product(matrix *mtx1, matrix *mtx2, matrix *prod);

int identity(matrix *m);

int isDiagonal(matrix *mtx);

double euclideanNormBetweenRows(matrix *mtx, double i, double j);

double euclideanNorm(matrix *mat, double i, double j);

int printDiagonal(matrix *mtx);

#endif /*SOFTWAREPROJECT_MATRIX_H*/
