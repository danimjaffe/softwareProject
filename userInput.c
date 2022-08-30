#include "userInput.h"

void invalid_input()
{
    printf("%s", "Invalid Input!");
    exit(1);
}

void general_error()
{
    printf("%s", "An Error Has Occurred");
    exit(1);
}

/*void validateInput(int argc, char *argv[]) {

}*/

/* Parses .txt file and returns a valid matrix with all the data in the .txt file*/
matrix *extractData(char *fileName)
{
    int rows = 0, cols = 0;
    matrix *data;
    countRowsAndCols(fileName, &rows, &cols);
    data = newMatrix(rows, cols);
    fillData(fileName, data);
    return data;
}

/* Counts the number of rows and columns in the input file*/

void countRowsAndCols(char *inputfile, int *rows, int *cols)
{
    FILE *fp = fopen(inputfile, "r");
    int c = 0;

    if (fp == NULL)
        general_error();

    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == '\n')
        {
            (*rows)++;
        }
        if (c == ',')
        {
            (*cols)++;
        }
    }
    *cols = (*cols) / (*rows) + 1;
    if (fclose(fp))
        general_error();
}

void fillData(char *inputfile, matrix *data)
{
    int inc, row = 1, col = 1;
    double val;
    char ch;
    FILE *fp = fopen(inputfile, "r");

    if (fp == NULL)
        general_error();

    while (EOF != (inc = fscanf(fp, "%lf%c", &val, &ch)) && inc == 2)
    {
        setElement(data, row, col, val);
        if (ch == ',')
        {
            col++;
        }
        else
        {
            col = 1;
            row++;
        }
    }
    if (fclose(fp))
        general_error();
}

void wamGoal(matrix *data, matrix **W)
{
    *W = weightedAdjacencyMatrix(data);
}

void ddgGoal(matrix *data, matrix **W, matrix **D)
{
    wamGoal(data, W);
    *D = DiagonalDegreeMatrix(*W, 0);
}

void lnormGoal(matrix *data, matrix **W, matrix **D, matrix **lNorm)
{
    wamGoal(data, W);
    *D = DiagonalDegreeMatrix(*W, 1);
    *lNorm = normalizedGraphLaplacian(*W, *D);
}

void jacobiGoal(matrix **A, matrix **V)
{
    jacobiAlgo(A, V);
}

void spkGoal(matrix *data, int * k, matrix **W, matrix **D, matrix **lNorm, matrix **V)
{
    int rows;
    matrix *newV;
    tuple *diagonal;
    lnormGoal(data, W, D, lNorm);
    jacobiAlgo(lNorm, V);
    printMatrix(*lNorm);
    rows = nRows(*lNorm);
    diagonal = (tuple *)malloc(rows * sizeof(tuple));
    sortEigenvalues(*lNorm, diagonal);
    *k = *k == 0 ? determineK(diagonal, rows) : *k;
    newV = newMatrix(rows, *k);
    reorderEigenvectors(*V, newV, diagonal);    
    deleteMatrix(*V);
    *V = newV;
    renormalizeEachRow(*V);
}

void runGoalC(char *goal, matrix *data, matrix **W, matrix **D, matrix **lNorm, matrix **A, matrix **V)
{
    matrix *res = NULL;
    if (strcmp(goal, "wam") == 0)
    {
        wamGoal(data, W);
        res = *W;
    }
    else if (strcmp(goal, "ddg") == 0)
    {
        ddgGoal(data, W, D);
        res = *D;
    }
    else if (strcmp(goal, "lnorm") == 0)
    {
        lnormGoal(data, W, D, lNorm);
        res = *lNorm;
    }
    else if (strcmp(goal, "jacobi") == 0)
    {
        *A = copyMatrix(data);
        jacobiGoal(A, V);
        printDiagonal(*A);
        res = *V;
    }
    else
    {
        invalid_input();
    }
    printMatrix(res);
}
