#include "rotationMatrix.h"

/* Creates a new rotation matrix
 */
rotationMatrix *newRotationMatrix(matrix *mtx, int pivotRow, int pivotCol, double c, double s) {
    rotationMatrix *m;
    m = (rotationMatrix *) malloc(sizeof(rotationMatrix));
    assert(m!=NULL);
    m->mtx=mtx;
    m->pivotRow = pivotRow;
    m->pivotCol = pivotCol;
    m->c = c;
    m->s = s;

    return m;
}


/* Deletes a rotation matrix matrix.  Returns 0 if successful and -1 if mtx
 * is NULL.
 */
int deleteRotationMatrix(rotationMatrix * rotMtx){
    if (!rotMtx) return -1;
    /* free rotMtx's mtx field */
    assert (rotMtx->mtx);
    deleteMatrix(rotMtx->mtx);
    /* free mtx itself */
    free(rotMtx);
    return 0;
}
