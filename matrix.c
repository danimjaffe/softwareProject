#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int rows;
    int cols;
    double * data;
} matrix;

matrix * newMatrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;

    // allocate a matrix structure
    matrix * m = (matrix *) malloc(sizeof(matrix));

    // set dimensions
    m->rows = rows;
    m->cols = cols;

    // allocate a double array of length rows * cols
    m->data = (double *) malloc(rows*cols*sizeof(double));
    // set all data to 0
    int i;
    for (i = 0; i < rows*cols; i++)
        m->data[i] = 0.0;

    return m;
}

/* Deletes a matrix.  Returns 0 if successful and -1 if mtx
 * is NULL.
 */
int deleteMatrix(matrix * mtx) {
    if (!mtx) return -1;
    // free mtx's data
    assert (mtx->data);
    free(mtx->data);

    // free mtx itself
    free(mtx);
    return 0;
}
