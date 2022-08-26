#include "userInput.h"

void invalid_input() {
    printf("%s", "Invalid Input!");
    exit(1);
}

void general_error() {
    printf("%s", "An Error Has Occurred");
    exit(1);
}


/*void validateInput(int argc, char *argv[]) {

}*/

matrix *extractData(char *fileName) {
    int rows, cols, i, j;
    matrix *data;
    printf("%s", fileName);
    rows = 4;
    cols = 4;
    data = newMatrix(rows, cols);

    for (i = 1; i <= rows; i++) {
        for (j = 1; j <= cols; j++) {
            setElement(data, i, j, (double) (i));
        }
    }
    return data;
}

void wamGoal(matrix *data, matrix **W) {
    *W = weightedAdjacencyMatrix(data);
}

void ddgGoal(matrix *data, matrix *W, matrix **D) {
    wamGoal(data, &W);
    *D = inverseSqrtDiagonalDegreeMatrix(W);
}

void lnormGoal(matrix *data, matrix *W, matrix *D, matrix **lNorm) {
    ddgGoal(data, W, &D);
    *lNorm = normalizedGraphLaplacian(W, D);
}

void jacobiGoal(matrix *data, matrix *W, matrix *D, matrix *lNorm, matrix *V) {
    lnormGoal(data, W, D, &lNorm);
    jacobiAlgo(&lNorm, &V);
}

void spkGoal(matrix *data, matrix *W, matrix *D, matrix *lNorm, matrix *V) {
    lnormGoal(data, W, D, &lNorm);
    jacobiAlgo(&lNorm, &V);
    /*TODO - continue algorithm */
}


void runGoalC(char *goal, matrix *data, matrix *W, matrix *D, matrix *lNorm, matrix *V) {
    matrix * res=NULL;
    if (strcmp(goal, "wam") == 0) {
        wamGoal(data, &W);
        res = W;
    } else if (strcmp(goal, "ddg") == 0) {
        ddgGoal(data, W, &D);
        res = D;
    } else if (strcmp(goal, "lnorm") == 0) {
        lnormGoal(data, W, D, &lNorm);
        res = lNorm;
    } else if (strcmp(goal, "jacobi") == 0) {
        jacobiGoal(data, W, D, lNorm, V);
        printDiagonal(V);
        res = lNorm;
    } else {
        invalid_input();
    }
    printMatrix(res);
}

void runGoalPy(char *goal, matrix *data) {
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL;
    if (strcmp(goal, "spk") == 0) {
        spkGoal(data, W, D, lNorm, V);
    } else {
        runGoalC(goal, data, W, D, lNorm, V);
    }
}
