#ifndef SOFTWAREPROJECT_ROTATION_MATRIX_H
#define SOFTWAREPROJECT_ROTATION_MATRIX_H

#include "matrix.h"

typedef struct {
    int pivotRow;
    int pivotCol;
    double c;
    double s;
    matrix * mtx;
} rotationMatrix;

rotationMatrix * newRotationMatrix(matrix* mtx, int pivotRow, int pivotCol,double c, double s);

#endif
