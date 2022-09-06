#include "eigengapHeuristic.h"

/*Determines k based on decreasing ordered eigenvalues*/

tuple newTuple(int idx, double val) {
    tuple t;
    t.idx = idx;
    t.val = val;
    return t;
}

/*Sort eigenvalues in decreasing order*/
void sortEigenvalues(matrix *A, tuple *diagonal) {
    int rows = nRows(A);
    int i;
    double val;
    for (i = 1; i <= rows; i++) {
        val = getElement(A, i, i);
        diagonal[i - 1] = newTuple(i, val);
    }

    qsort(diagonal, rows, sizeof(tuple), compareTuple);

}

/*Compares 2 tuples*/
int compareTuple(const void *p1, const void *p2) {
    const tuple *q1 = p1, *q2 = p2;
    double val;
    if ((*q1).val == (*q2).val) {
        return (int) ((*q1).idx - (*q2).idx);
    }

    val = ((*q1).val - (*q2).val) > 0 ? -1 : 1;
    return val;
}

/* Reorder eigenvectors of V*/
void reorderEigenvectors(matrix *V, matrix *U, tuple *diagonal) {
    int rows = nRows(U), cols = nCols(U), i, j;
    for (j = 1; j <= cols; j++) {
        for (i = 1; i <= rows; i++) {
            setElement(U, i, j, getElement(V, i, diagonal[j - 1].idx));
        }
    }
}

/*Determine k based on heuristic eigengap theory*/
int determineK(tuple *diagonal, int rows) {
    int i;
    int n = (int)(rows / 2), k = 1;
    double temp, maxDelta = 0.0;

    for (i = 0; i < n - 1; i++) {
        temp = fabs(diagonal[i].val - diagonal[i+1].val);
        if (temp > maxDelta) {
            maxDelta = temp;
            k = i + 1;
        }
    }

    return k;
}

/* Renormalize V to have unit length*/
void renormalizeEachRow(matrix *V) {
    int rows = nRows(V), cols = nCols(V);
    int i, j;
    double val;
    for (i = 1; i <= rows; i++) {
        val = 0.0;
        for (j = 1; j <= cols; j++) {
            val += getElement(V, i, j) * getElement(V, i, j);
        }
        if (val != 0) {
            val = sqrt(val);
            for (j = 1; j <= cols; j++) {
                setElement(V, i, j, getElement(V, i, j) / val);
            }
        }
    }
}
