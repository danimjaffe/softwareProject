#include "rotationMatrix.h"

/*TODO: Add general errors when creating new rotation matrix*/

/* Creates a new rotation matrix set to identity matrix of size(A)
 */
rotationMatrix *newRotationMatrix(matrix *A) {
    rotationMatrix *P;
    matrix *data = newMatrix(nRows(A), nCols(A));
    P = (rotationMatrix *) malloc(sizeof(rotationMatrix));
    assert(P != NULL);
    identity(data);
    P->mtx = data;
    P->pivotRow = 1;
    P->pivotCol = 2;
    P->c = 1.0;
    P->s = 0.0;
    return P;
}

/* updates rotation matrix P values by matrix A
 */
void updateRotationMatrixP(matrix *A, rotationMatrix *P) {
    setPivotElem(A, P);
    setRotationMatrixSC(A, P);
    /* Create P rotation matrix based on c and s values*/
    setElement(P->mtx, P->pivotRow, P->pivotRow, P->c);
    setElement(P->mtx, P->pivotCol, P->pivotCol, P->c);
    setElement(P->mtx, P->pivotRow, P->pivotCol, P->s);
    setElement(P->mtx, P->pivotCol, P->pivotRow, -(P->s));
}

void setPivotElem(matrix *A, rotationMatrix *P) {
    double maxVal = fabs(getElement(A, P->pivotRow, P->pivotCol));
    int i, j, rows = nRows(A), cols = nCols(A);
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            double currAbsElem = fabs(getElement(A, i, j));
            if (i != j && maxVal < currAbsElem) {
                P->pivotRow = i;
                P->pivotCol = j;
                maxVal = currAbsElem;
            }
        }
    }
}

void setRotationMatrixSC(matrix *A, rotationMatrix *P) {
    double theta, signTheta, t, c;
    theta = (getElement(A, P->pivotCol, P->pivotCol) - getElement(A, P->pivotRow, P->pivotRow)) /
            (2 * getElement(A, P->pivotRow, P->pivotCol));
    signTheta = (theta >= 0) ? 1.0 : -1.0;
    t = signTheta / (fabs(theta) + sqrt(theta * theta + 1));
    c = 1 / (sqrt(t * t + 1));
    P->c = c;
    P->s = t * c;
}

void resetRotationMatrix(rotationMatrix *P) {
    int pivotRow = P->pivotRow, pivotCol = P->pivotCol;
    setElement(P->mtx, pivotRow, pivotRow, 1.0);
    setElement(P->mtx, pivotCol, pivotCol, 1.0);
    setElement(P->mtx, pivotRow, pivotCol, 0.0);
    setElement(P->mtx, pivotCol, pivotRow, 0.0);
    P->pivotRow = 1;
    P->pivotCol = 2;
    P->c = 1.0;
    P->s = 0.0;
}

/* Deletes a rotation matrix matrix.  Returns 0 if successful and -1 if mtx
 * is NULL.
 */
int deleteRotationMatrix(rotationMatrix *rotMtx) {
    if (!rotMtx) return -1;
    /* free rotMtx's mtx field */
    assert (rotMtx->mtx);
    deleteMatrix(rotMtx->mtx);
    /* free mtx itself */
    free(rotMtx);
    return 0;
}
