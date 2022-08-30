#include "graphRepresentation.h"

/*TODO: free alocated memory*/

/* Creates and returns the weighted adjacency matrix of mat
 */

matrix *weightedAdjacencyMatrix(matrix *mat) {
    int nDataPoints = nRows(mat);
    int i, j;
    matrix *weighted;
    double norm, W_ij;
    weighted = newMatrix(nDataPoints, nDataPoints);
    for (i = 1; i <= weighted->rows; i++) {
        for (j = i + 1; j <= weighted->rows; j++) {
            norm = euclideanNormBetweenRows(mat, i, j);
            W_ij = exp((-norm) / 2);
            setElement(weighted, i, j, W_ij);
            setElement(weighted, j, i, W_ij);
        }
    }
    return weighted;
}

/*TODO: free allocated memory*/

/* Creates and returns inverse sqrt of diagonal degree matrix mtx
    when called by ddg than inversedSquareRoot = 0 otherwise inversedSquareRoot=1
 */

matrix *DiagonalDegreeMatrix(matrix *mtx, int inversedSquareRoot) {
    int rows = nRows(mtx);
    int cols = nCols(mtx);
    matrix *D = newMatrix(rows, cols);
    int i, j;
    for (i = 1; i <= rows; i++) {
        double rowSum = 0;
        for (j = 1; j <= cols; j++) {
            rowSum += getElement(mtx, i, j);
        }
        rowSum = (inversedSquareRoot == 0) ? rowSum : 1 / sqrt(rowSum);
        setElement(D, i, i, rowSum);
    }
    return D;
}

/*TODO: free allocated memory*/

/* Creates and returns normalized graph Laplacian
 */

matrix *normalizedGraphLaplacian(matrix *W, matrix *D) {
    int rows = nRows(W);
    int cols = nCols(W);
    matrix *Lnorm, *I, *temp;
    Lnorm = newMatrix(rows, cols);
    I = newMatrix(rows, cols);
    temp = newMatrix(rows, cols);
    identity(I);
    product(D, W, temp);
    product(temp, D, Lnorm);
    sub(I, Lnorm, Lnorm);
    deleteMatrix(I);
    deleteMatrix(temp);
    return Lnorm;
}

void jacobiAlgo(matrix **A, matrix **V) {
    int rows = nRows(*A), cols = nCols(*A);
    matrix *temp, *ATag, *temp2 = NULL;
    int convergence = isDiagonal(*A);
    int maxRotations = 100, rotationNumber = 1;
    rotationMatrix *P = newRotationMatrix(*A);
    *V = newMatrix(rows, cols);
    identity(*V);
    temp = newMatrix(rows, cols);
    /*TODO: case when initial matrix is diagonal*/
    while (convergence != 1 && rotationNumber <= maxRotations) {
        resetRotationMatrix(P);
        updateRotationMatrixP(*A, P);
        ATag = transformAToATag(*A, P);
        convergence = checkOffConvergence(*A, ATag);
        deleteMatrix(*A);
        *A = ATag;
        product(*V, P->mtx, temp);
        temp2 = *V;
        *V = temp;
        temp = temp2;
        rotationNumber++;
    }
    
    deleteMatrix(temp);
    deleteRotationMatrix(P);
    
}

/*Returns A' based on initial A matrix*/
matrix *transformAToATag(matrix *A, rotationMatrix *P) {
    int rows = nRows(A);
    int r, i = P->pivotRow, j = P->pivotCol;
    matrix *ATag;
    double val, ATag_ij = 0.0;
    double c = P->c, s = P->s;
    ATag = copyMatrix(A);
    for (r = 1; r <= rows; r++) {
        if (r != j && r != i) {
            /*A'ri and A'ir*/
            val = (c * getElement(A, r, i)) - (s * getElement(A, r, j));
            setElement(ATag, r, i, val);
            setElement(ATag, i, r, val);
            /*A'rj and A'jr*/
            val = (c * getElement(A, r, j)) + (s * getElement(A, r, i));
            setElement(ATag, r, j, val);
            setElement(ATag, j, r, val);
        }
    }
    /*A'ii*/
    val = (c * c * getElement(A, i, i)) + (s * s * getElement(A, j, j)) - (2 * s * c * getElement(A, i, j));
    setElement(ATag, i, i, val);
    /*A'jj*/
    val = (s * s * getElement(A, i, i)) + (c * c * getElement(A, j, j)) + (2 * s * c * getElement(A, i, j));
    setElement(ATag, j, j, val);
    /*A'ij and A'ji*/
    setElement(ATag, i, j, ATag_ij);
    setElement(ATag, j, i, ATag_ij);
    return ATag;
}

/*Checks if there is a convergence: off(A)^2 - off(A')^2<=epsilon
returns 1 if there is a convergence 0 otherwise*/
int checkOffConvergence(matrix *A, matrix *ATag) {
    int rows = nRows(A);
    int cols = nCols(A);
    double eps = 0.00001;
    double offA = 0.0, offATag = 0.0;
    int i, j;
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            if (i != j) {
                offA += getElement(A, i, j) * getElement(A, i, j);
                offATag += getElement(ATag, i, j) * getElement(ATag, i, j);
            }
        }
    }
    return (offA - offATag) <= eps;
}
