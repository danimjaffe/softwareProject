#ifndef SOFTWAREPROJECT_GRAPH_REPRESENTATION_H
#define SOFTWAREPROJECT_GRAPH_REPRESENTATION_H

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

void jacobiAlgo(matrix **A, matrix **V);

#endif
