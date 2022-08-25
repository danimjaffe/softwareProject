#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "matrix.h"
#include "rotationMatrix.h"
#include "jacobi.h"

matrix *weightedAdjacencyMatrix(matrix *mat);

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx);

matrix *normailzedGraphLaplacian(matrix *W, matrix *D);

rotationMatrix *createRotationMatrixP(matrix *A);

jacobi_s * jacobi(matrix *A);

matrix *transformAToATag(matrix *A, rotationMatrix *P);

int checkConvergence(matrix *A, matrix *ATag);

int main()

{
    int nRows, nCols, i, j;
    matrix *data, *W, *D, *Lnorm;
    rotationMatrix *P;

    nRows = 4;
    nCols = 3;
    data = newMatrix(nRows, nCols);

    for (i = 1; i <= nRows; i++)
    {
        for (j = 1; j <= nCols; j++)
        {
            setElement(data, i, j, (double)(i));
        }
    }
    W = weightedAdjacencyMatrix(data);
    D = inverseSqrtDiagonalDegreeMatrix(W);
    Lnorm = normailzedGraphLaplacian(W, D);
    P = createRotationMatrixP(Lnorm);
    printMatrix(P->mtx);
    return 0;
}

/*TODO: free alocated memory*/

/* Creates and returns the weighted adjacency matrix of mat
 */

matrix *weightedAdjacencyMatrix(matrix *mat)
{
    int nDataPoints = nRows(mat);
    int i, j;
    matrix *weighted;
    double norm, W_ij;
    weighted = newMatrix(nDataPoints, nDataPoints);
    for (i = 1; i <= weighted->rows; i++)
    {
        for (j = i + 1; j <= weighted->rows; j++)
        {
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

matrix *inverseSqrtDiagonalDegreeMatrix(matrix *mtx)
{
    int rows = nRows(mtx);
    int cols = nCols(mtx);
    matrix *D = newMatrix(rows, cols);
    int i, j;
    for (i = 1; i <= rows; i++)
    {
        double rowSum = 0;
        for (j = 1; j <= cols; j++)
        {
            rowSum += getElement(mtx, i, j);
        }
        setElement(D, i, i, 1 / sqrt(rowSum));
    }
    return D;
}

/*TODO: free alocated memory*/

/* Creates and returns normalized graph Laplacian
 */

matrix *normailzedGraphLaplacian(matrix *W, matrix *D)
{
    int rows = nRows(W);
    int cols = nCols(W);
    matrix *Lnorm, *I, *temp1, *temp2;
    Lnorm = newMatrix(rows, cols);
    I = newMatrix(rows, cols);
    temp1 = newMatrix(rows, cols);
    temp2 = newMatrix(rows, cols);
    identity(I);
    product(D, W, temp1);
    product(temp1, D, temp2);
    sub(I, temp2, Lnorm);
    deleteMatrix(I);
    deleteMatrix(temp1);
    deleteMatrix(temp2);
    return Lnorm;
}

jacobi_s * jacobi(matrix *A)
{
    jacobi_s * j;
    int rows = nRows(A);
    int cols = nCols(A);
    matrix *temp, *ATag, *V;
    int convergence = isDiagonal(A);
    int maxRotations = 100;
    int rotationNumber = 1;
    rotationMatrix *P;

    V = newMatrix(rows, cols);
    identity(V);

    /*TODO: case when initial matrix is diagonal*/
    while (convergence != 1 && rotationNumber <= maxRotations)
    {
        P = createRotationMatrixP(A);
        ATag = transformAToATag(A, P);
        convergence = checkConvergence(A, ATag);
        deleteMatrix(A);
        A = ATag;

        temp = newMatrix(rows, cols);
        product(V, P->mtx, temp);
        V = temp;
        deleteMatrix(temp);

        deleteRotationMatrix(P);
        rotationNumber++;
    }
    
    j = newJacobi(A,V);
    return j;
}

/* Creates rotation matrix P from initial matrix A
 */
rotationMatrix *createRotationMatrixP(matrix *A)
{
    rotationMatrix *P;
    matrix *mtx;
    int i, j, pivotRow, pivotCol;
    double maxVal, theta, signTheta, t, c, s;
    int rows = nRows(A);
    int cols = nCols(A);

    /*find pivot element indices*/

    pivotRow = 1;
    pivotCol = 2;
    maxVal = fabs(getElement(A, 1, 2));

    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= cols; j++)
        {

            if (i != j && maxVal < fabs(getElement(A, i, j)))
            {
                pivotRow = i;
                pivotCol = j;
                maxVal = fabs(getElement(A, i, j));
            }
        }
    }

    /* obtain c and s*/
    theta = (getElement(A, pivotRow, pivotRow) - getElement(A, pivotCol, pivotCol)) /
            (2 * getElement(A, pivotRow, pivotCol));
    signTheta = (theta >= 0) ? 1 : -1;
    t = signTheta / (fabs(theta) + sqrt(theta * theta + 1));
    c = 1 / (sqrt(t * t + 1));
    s = t * c;

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

/*Returns A' based on initial A matrix*/
matrix *transformAToATag(matrix *A, rotationMatrix *P)
{
    int rows = nRows(A);
    int cols = nCols(A);
    int r;
    int i = P->pivotRow, j = P->pivotCol;
    matrix *ATag;
    double ATag_ri, ATag_rj, ATag_ii, ATag_jj, ATag_ij = 0.0;
    double c = P->c, s = P->s;
    ATag = newMatrix(rows, cols);

    for (r = 1; r <= rows; r++)
    {
        if (r != j && r != i)
        {
            /*A'ri and A'ir*/
            ATag_ri = c * getElement(A, r, i) - s * getElement(A, r, j);
            setElement(ATag, r, i, ATag_ri);
            setElement(ATag, i, r, ATag_ri);

            /*A'rj and A'jr*/

            ATag_rj = c * getElement(A, r, j) + s * getElement(A, r, i);
            setElement(ATag, r, j, ATag_rj);
            setElement(ATag, j, r, ATag_rj);
        }
    }

    /*A'ii*/
    ATag_ii = c * c * getElement(A, i, i) + s * s * getElement(A, j, j) - 2 * s * c * getElement(A, i, j);
    setElement(ATag, i, i, ATag_ii);

    /*A'jj*/
    ATag_jj = s * s * getElement(A, i, i) + c * c * getElement(A, j, j) + 2 * s * c * getElement(A, i, j);
    setElement(ATag, j, j, ATag_jj);

    /*A'ij and A'ji*/
    setElement(ATag, i, j, ATag_ij);
    setElement(ATag, j, i, ATag_ij);

    return ATag;
}

/*Checks if there is a convergence: off(A)^2 - off(A')^2<=epsilon
retruns 1 if there is a convergence 0 otherwise*/
int checkConvergence(matrix *A, matrix *ATag)
{
    int rows = nRows(A);
    int cols = nCols(A);
    double eps = 0.00001;
    double offA = 0.0, offATag = 0.0;
    int i, j;
    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= cols; j++)
        {
            if (i != j)
            {
                offA = offA + getElement(A, i, j) * getElement(A, i, j);
                offATag = offATag + getElement(ATag, i, j) * getElement(ATag, i, j);
            }
        }
    }

    return (offA - offATag) <= eps;
}
