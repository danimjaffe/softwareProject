#ifndef SOFTWAREPROJECT_EIGENGAP_HEURISTIC_H
#define SOFTWAREPROJECT_EIGENGAP_HEURISTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "rotationMatrix.h"

typedef struct
{
    int idx;
    double val;
} tuple;

tuple newTuple(int idx, double val);

void sortEigenvalues(matrix *A, tuple *diagonal);

int compareTuple(const void *p1, const void *p2);

int determineK(tuple *diagonal, int rows);

void reorderEigenvectors(matrix *V, matrix *newV, tuple *diagonal);

void renormalizeEachRow(matrix *V);

#endif
