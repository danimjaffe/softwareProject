#ifndef SOFTWAREPROJECT_SPKMEANS_H
#define SOFTWAREPROJECT_SPKMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "rotationMatrix.h"

matrix *weightedAdjacencyMatrix(matrix *mat);

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx);

matrix *normalizedGraphLaplacian(matrix *W, matrix *D);

rotationMatrix *createRotationMatrixP(matrix *A);

matrix *transformAToATag(matrix *A, rotationMatrix *P);

int checkOffConvergence(matrix *A, matrix *ATag);

void getPivotElem(matrix *A, int *pivotRow, int *pivotCol);

void getRotationMatrixSC(matrix *A, int pivotRow, int pivotCol, double *s, double *c);

#endif
