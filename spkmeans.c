#include "spkmeans.h"

int main(int argc, char *argv[])
{
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL, *A = NULL, *data;
    if (argc != 3)
    {
        invalid_input();
    }
    data = extractData(argv[2]);
    runGoalC(argv[1], data, &W, &D, &lNorm, &A, &V);
    deleteMatrix(W);
    deleteMatrix(D);
    deleteMatrix(lNorm);
    deleteMatrix(A);
    deleteMatrix(V);
    deleteMatrix(data);
    return 0;
}
