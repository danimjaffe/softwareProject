#include "rotationMatrix.h"

/* Creates a new rotation matrix
 */
rotationMatrix *newRotationMatrix(matrix *mtx, int pivotRow, int pivotCol, double c, double s) {
    rotationMatrix *m;
    m = (rotationMatrix *) malloc(sizeof(rotationMatrix));
    assert(m != NULL);
    m->pivotRow = pivotRow;
    m->pivotCol = pivotCol;
    m->c = c;
    m->s = s;

    return m;
}
