#ifndef SOFTWAREPROJECT_ROTATION_MATRIX_H
#define SOFTWAREPROJECT_ROTATION_MATRIX_H

#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct
{
    int pivotRow;
    int pivotCol;
    double c;
    double s;
    matrix *mtx;
} rotationMatrix;

rotationMatrix *newRotationMatrix(matrix *mtx, int pivotRow, int pivotCol, double c, double s);
int deleteRotationMatrix(rotationMatrix * rotMtx);

#endif
