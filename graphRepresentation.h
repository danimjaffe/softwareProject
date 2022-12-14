/* Implementation of Section 1.1 - Graph Representation */

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

matrix *DiagonalDegreeMatrix(matrix *mtx, int inversedSquareRoot);

matrix *normalizedGraphLaplacian(matrix *W, matrix *D);

void jacobiAlgo(matrix **A, matrix **V);

matrix *transformAToATag(matrix *A, rotationMatrix *P);

int checkOffConvergence(matrix *A, matrix *ATag);

#endif
