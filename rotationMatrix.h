#ifndef SOFTWAREPROJECT_ROTATION_MATRIX_H
#define SOFTWAREPROJECT_ROTATION_MATRIX_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "matrix.h"

typedef struct {
    int pivotRow;
    int pivotCol;
    double c;
    double s;
    matrix *mtx;
} rotationMatrix;

rotationMatrix *newRotationMatrix(matrix *A);

void setPivotElem(matrix *A, rotationMatrix *P);

void setRotationMatrixSC(matrix *A, rotationMatrix *P);

void resetRotationMatrix(rotationMatrix *P);

void updateRotationMatrixP(matrix *A, rotationMatrix *P);

#endif
