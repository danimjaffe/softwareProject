#include "jacobi.h"

/* Creates a new rotation matrix
 */
jacobi_s *newJacobi(matrix *A, matrix *V) {
    jacobi_s *j;
    j = (jacobi_s *) malloc(sizeof(jacobi_s));
    assert(j!=NULL);
    j->A = A;
    j->V = V;
    return j;
}


/* Deletes a rotation matrix matrix.  Returns 0 if successful and -1 if mtx
 * is NULL.
 */
int deleteJacobi(jacobi_s * j){
    if (!j) return -1;
    /* free j's A field */
    assert (j->A);
    deleteMatrix(j->A);
    /* free j's V field */
    assert (j->V);
    deleteMatrix(j->V);
    /* free mtx itself */
    free(j);
    return 0;
}
