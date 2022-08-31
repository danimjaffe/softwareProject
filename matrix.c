#include "matrix.h"

/*TODO: Add general errors when creating new matrix*/

/* Creates a ``rows by cols'' matrix with all values 0.
 * Returns NULL if rows <= 0 or cols <= 0 and otherwise a
 * pointer to the new matrix.
 */
matrix *newMatrix(int rows, int cols) {
    matrix *m;
    int i;
    if (rows <= 0 || cols <= 0) return NULL;

    /* allocate a matrix structure*/
    m = (matrix *) malloc(sizeof(matrix));
    if (m==NULL)
    {
        general_error();
    }
    
    /*assert(m != NULL);*/

    /* set dimensions*/
    m->rows = rows;
    m->cols = cols;

    /* allocate a double array of length rows * cols */
    m->data = (double *) malloc(rows * cols * sizeof(double));
    if (m->data==NULL)
    {
        general_error();
    }
    /*assert(m->data);*/
    /* set all data to 0 */
    for (i = 0; i < rows * cols; i++)
        m->data[i] = 0.0;

    return m;
}

/* Deletes a matrix.  Returns 0 if successful and -1 if mtx
 * is NULL.
 */
int deleteMatrix(matrix *mtx) {
    if (!mtx) return -1;
    /* free mtx's data */
    if (mtx->data==NULL)
    {
        general_error();
    }
    /*assert (mtx->data);*/
    free(mtx->data);
    /* free mtx itself */
    free(mtx);
    return 0;
}

#define ELEM(mtx, row, col) \
  mtx->data[(col-1) * mtx->rows + (row-1)]

/* Copies a matrix.  Returns NULL if mtx is NULL.
 */
matrix *copyMatrix(matrix *mtx) {
    matrix *cp;
    if (!mtx) return NULL;

    /* create a new matrix to hold the copy */
    cp = newMatrix(mtx->rows, mtx->cols);

    /* copy mtx's data to cp's data */
    memcpy(cp->data, mtx->data,
           mtx->rows * mtx->cols * sizeof(double));

    return cp;
}

/* Sets the (row, col) element of mtx to val.  Returns 0 if
 * successful, -1 if mtx is NULL, and -2 if row or col are
 * outside of the dimensions of mtx.
 */
int setElement(matrix *mtx, int row, int col, double val) {
    if (!mtx) return -1;
    if (mtx->data==NULL)
    {
        general_error();
    }
    /*assert (mtx->data);*/
    if (row <= 0 || row > mtx->rows ||
        col <= 0 || col > mtx->cols)
        return -2;

    ELEM(mtx, row, col) = val;
    return 0;
}

/* returns the element in mtx[row][col]
 */
double getElement(matrix *mtx, int row, int col) {
    return ELEM(mtx, row, col);
}

/* returns number of rows in matrix
 */
int nRows(matrix *mtx) {
    return mtx->rows;
}

/* returns number of columns in matrix
 */
int nCols(matrix *mtx) {
    return mtx->cols;
}

/* Prints the matrix to stdout.  Returns 0 if successful
 * and -1 if mtx is NULL.
 */

int printMatrix(matrix *mtx) {
    int row, col;
    if (!mtx) return -1;
    for (row = 1; row <= mtx->rows; row++) {
        printf("%.4f", ELEM(mtx, row, 1));
        for (col = 2; col <= mtx->cols; col++) {
            printf(",%.4f", ELEM(mtx, row, col));
        }
        printf("\n");
    }
    return 0;
}

/* Writes the sub of matrices mtx1 from mtx2 into matrix
 * sub. Returns 0 if successful, -1 if any of the matrices
 * are NULL, and -2 if the dimensions of the matrices are
 * incompatible.
 */
int sub(matrix *mtx1, matrix *mtx2, matrix *sub) {
    int row, col;
    if (!mtx1 || !mtx2 || !sub) return -1;
    if (mtx1->rows != mtx2->rows ||
        mtx1->rows != sub->rows ||
        mtx1->cols != mtx2->cols ||
        mtx1->cols != sub->cols)
        return -2;

    for (col = 1; col <= mtx1->cols; col++)
        for (row = 1; row <= mtx1->rows; row++)
            ELEM(sub, row, col) =
                    ELEM(mtx1, row, col) - ELEM(mtx2, row, col);
    return 0;
}

/* Writes the product of matrices mtx1 and mtx2 into matrix
 * prod.  Returns 0 if successful, -1 if any of the
 * matrices are NULL, and -2 if the dimensions of the
 * matrices are incompatible.
 */
int product(matrix *mtx1, matrix *mtx2, matrix *prod) {
    int row, col, k;
    if (!mtx1 || !mtx2 || !prod) return -1;
    if (mtx1->cols != mtx2->rows ||
        mtx1->rows != prod->rows ||
        mtx2->cols != prod->cols)
        return -2;

    for (col = 1; col <= mtx2->cols; col++)
        for (row = 1; row <= mtx1->rows; row++) {
            double val = 0.0;
            for (k = 1; k <= mtx1->cols; k++)
                val += ELEM(mtx1, row, k) * ELEM(mtx2, k, col);
            ELEM(prod, row, col) = val;
        }
    return 0;
}


int identity(matrix *m) {
    int row, col;
    if (!m || m->rows != m->cols) return -1;
    for (col = 1; col <= m->cols; col++)
        for (row = 1; row <= m->rows; row++)
            if (row == col)
                ELEM(m, row, col) = 1.0;
            else
                ELEM(m, row, col) = 0.0;
    return 0;
}

/*Checks if mtx is square matrix*/
int isSquare(matrix *mtx) {
    return mtx && mtx->rows == mtx->cols;
}

/*Checks if mtx is diagonal matrix*/
int isDiagonal(matrix *mtx) {
    int row, col;
    if (!isSquare(mtx)) return 0;
    for (col = 1; col <= mtx->cols; col++)
        for (row = 1; row <= mtx->rows; row++)
            /* if the element is not on the diagonal and not 0 */
            if (row != col && ELEM(mtx, row, col) != 0.0)
                /* then the matrix is not diagonal */
                return 0;
    return 1;
}


/* calculates Euclidean Norm between the vectors in rows i, j in the given mtx.
*/

double euclideanNormBetweenRows(matrix *mtx, double i, double j) {
    double dis = 0.0;
    double temp;
    int k;
    for (k = 1; k <= mtx->cols; k++) {
        temp = getElement(mtx, i, k) - getElement(mtx, j, k);
        dis = dis + temp * temp;
    }
    return sqrt(dis);
}


/* Prints the diagonal values of the matrix.
 * Returns 0 if successful, -1 if matrix is not square,
 * and -2 if the dimensions of the matrices are
 * incompatible.
 */
int printDiagonal(matrix *mtx) {
    int rows = nRows(mtx), i;
    if (!isSquare(mtx)) return -2;
    printf("%.4f", getElement(mtx, 1, 1));
    for (i = 2; i <= rows; i++) {
        printf(",%.4f", getElement(mtx, i, i));
    }
    printf("\n");
    return 0;
}
