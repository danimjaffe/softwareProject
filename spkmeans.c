#include "spkmeans.h"

/* TODO - Error Handling */
int main(int argc, char *argv[]) {
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL, *data;
    if (argc != 3) {
        invalid_input();
    }
    data = extractData(argv[2]);
    runGoalC(argv[1], data, &W, &D, &lNorm, &V);
    deleteMatrix(W);
    deleteMatrix(D);
    deleteMatrix(lNorm);
    deleteMatrix(V);
    deleteMatrix(data);
    return 0;
}

/*
int main() {

    
    int rows, cols, i, j;
    matrix *data, *W, *D, *Lnorm, *V;
    rotationMatrix *P;

    rows = 4;
    cols = 4;
    data = newMatrix(rows, cols);

    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            setElement(data, i, j, (double) (i));
        }
    }
    W = weightedAdjacencyMatrix(data);
    D = inverseSqrtDiagonalDegreeMatrix(W);
    Lnorm = normalizedGraphLaplacian(W, D);
    P = createRotationMatrixP(Lnorm);
    V = newMatrix(rows, cols);
    identity(V);
    printMatrix(P->mtx);
    

   int rows, cols, i, j;
    matrix *data,*dataCP, *newData;

    rows = 4;
    cols = 4;
    data = newMatrix(rows, cols);

    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            setElement(data, i, j, (double) (i*j));
        }
    }

    dataCP = copyMatrix(data);
    newData = newMatrix(rows,cols);
    sortEigenvaluesAndVectors(data,dataCP,newData);

    return 0;
}
*/
