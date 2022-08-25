#ifndef SOFTWAREPROJECT_GRAPHREPRESENTATION_H
#define SOFTWAREPROJECT_GRAPHREPRESENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "rotationMatrix.h"
#include "jacobi.h"

matrix *weightedAdjacencyMatrix(matrix *mat);

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx);

matrix *normailzedGraphLaplacian(matrix *W, matrix *D);

rotationMatrix *createRotationMatrixP(matrix *A);

jacobi_s *jacobi(matrix *A);

matrix *transformAToATag(matrix *A, rotationMatrix *P);

int checkConvergence(matrix *A, matrix *ATag);

#endif