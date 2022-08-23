#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx);


matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx) {
    int rows = nRows(mtx);
    int cols = nCols(mtx);
    matrix *D = newMatrix(rows, cols);
    int i, j;
    for (i = 1; i <= rows; i++) {
        double rowSum = 0;
        for (j = 1; j <= cols; j++) {
            rowSum += getElement(mtx, i, j);
        }
        setElement(D, i, i, 1/ sqrt(rowSum));
    }
    return D;
}




int main(){
    matrix * A, * D;
    int i, j;
    A = newMatrix(5,5);
    for (i = 1; i <= 5; ++i) {
        for (j = 1; j <= 5; ++j) {
            setElement(A,i,j,i);
        }
    }
    D = inverseSqrtDiagonalDegreeMatrix(A);
    printMatrix(D);
}
