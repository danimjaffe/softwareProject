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

/*TODO: free alocated memory*/

/* Creates and returns inverse sqrt of diagonal degree matrix mtx
 */

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx) {
    int rows = nRows(mtx);
    int cols = nCols(mtx);
    matrix *D = newMatrix(rows, cols);
    int i, j;
    for (i = 1; i <= rows; i++) {
        double rowSum = 0;
        for (j = 1; j <= cols; j++) {
            rowSum += getElement(mtx, i, j);
        }
        setElement(D, i, i, 1 / sqrt(rowSum));
    }
    return D;
}

/*TODO: free alocated memory*/

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
    matrix *temp, *ATag;
    int convergence = isDiagonal(*A);
    int maxRotations = 100, rotationNumber = 1;
    rotationMatrix *P;
    /*TODO: case when initial matrix is diagonal*/
    while (convergence != 1 && rotationNumber <= maxRotations) {
        P = createRotationMatrixP(*A);
        ATag = transformAToATag(*A, P);
        convergence = checkOffConvergence(*A, ATag);
        deleteMatrix(*A);
        *A = ATag;
        temp = newMatrix(rows, cols);
        product(*V, P->mtx, temp);
        *V = temp;
        deleteMatrix(temp);
        deleteRotationMatrix(P);
        rotationNumber++;
    }
}

/* Creates rotation matrix P from initial matrix A
 */
rotationMatrix *createRotationMatrixP(matrix *A) {
    rotationMatrix *P;
    matrix *mtx;
    int pivotRow, pivotCol, rows = nRows(A), cols = nCols(A);
    double c, s;
    /*find pivot element indices*/
    getPivotElem(A, &pivotRow, &pivotCol);
    /* obtain c and s*/
    getRotationMatrixSC(A, pivotRow, pivotCol, &s, &c);
    /* Create P rotation matrix based on c and s values*/
    mtx = newMatrix(rows, cols);
    identity(mtx);
    setElement(mtx, pivotRow, pivotRow, c);
    setElement(mtx, pivotCol, pivotCol, c);
    setElement(mtx, pivotRow, pivotCol, s);
    setElement(mtx, pivotCol, pivotRow, -s);

    P = newRotationMatrix(mtx, pivotRow, pivotCol, c, s);
    return P;
}

void getPivotElem(matrix *A, int *pivotRow, int *pivotCol) {
    double maxVal = fabs(getElement(A, 1, 2));
    int i, j, rows = nRows(A), cols = nCols(A);
    *pivotRow = 1;
    *pivotCol = 2;
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            double currAbsElem = fabs(getElement(A, i, j));
            if (i != j && maxVal < currAbsElem) {
                *pivotRow = i;
                *pivotCol = j;
                maxVal = currAbsElem;
            }
        }
    }
}

void getRotationMatrixSC(matrix *A, int pivotRow, int pivotCol, double *s, double *c) {
    double theta, signTheta, t;
    theta = (getElement(A, pivotRow, pivotRow) - getElement(A, pivotCol, pivotCol)) /
            (2 * getElement(A, pivotRow, pivotCol));
    signTheta = (theta >= 0) ? 1 : -1;
    t = signTheta / (fabs(theta) + sqrt(theta * theta + 1));
    *c = 1 / (sqrt(t * t + 1));
    *s = t * (*c);
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
            val = c * getElement(A, r, i) - s * getElement(A, r, j);
            setElement(ATag, r, i, val);
            setElement(ATag, i, r, val);
            /*A'rj and A'jr*/
            val = c * getElement(A, r, j) + s * getElement(A, r, i);
            setElement(ATag, r, j, val);
            setElement(ATag, j, r, val);
        }
    }
    /*A'ii*/
    val = c * c * getElement(A, i, i) + s * s * getElement(A, j, j) - 2 * s * c * getElement(A, i, j);
    setElement(ATag, i, i, val);
    /*A'jj*/
    val = s * s * getElement(A, i, i) + c * c * getElement(A, j, j) + 2 * s * c * getElement(A, i, j);
    setElement(ATag, j, j, val);
    /*A'ij and A'ji*/
    setElement(ATag, i, j, ATag_ij);
    setElement(ATag, j, i, ATag_ij);
    return ATag;
}

/*Checks if there is a convergence: off(A)^2 - off(A')^2<=epsilon
retruns 1 if there is a convergence 0 otherwise*/
int checkOffConvergence(matrix *A, matrix *ATag) {
    int rows = nRows(A);
    int cols = nCols(A);
    double eps = 0.00001;
    double offA = 0.0, offATag = 0.0;
    int i, j;
    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            if (i != j) {
                offA = offA + getElement(A, i, j) * getElement(A, i, j);
                offATag = offATag + getElement(ATag, i, j) * getElement(ATag, i, j);
            }
        }
    }

    return (offA - offATag) <= eps;
}

/*Determines k based on decreasing ordered eigenvalues*/

int determineK(matrix * A){
    int i, rows=nRows(A);
    int n=floor(rows/2), k=1;
    double temp,maxDelta = 0.0;

    for (i = 1; i <=n-1; i++)
    {
        temp = fabs(getElement(A,i,i)-getElement(A,i+1,i+1));
        if (temp>maxDelta)
        {
            maxDelta=temp;
            k=i;
        }
        
    }
    return k;    
}