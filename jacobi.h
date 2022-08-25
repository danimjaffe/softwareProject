#ifndef SOFTWAREPROJECT_JACOBI_H
#define SOFTWAREPROJECT_JACOBI_H

#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

typedef struct
{
    matrix *A;
    matrix *V;
} jacobi_s;

jacobi_s *newJacobi(matrix *A, matrix *V);

#endif
