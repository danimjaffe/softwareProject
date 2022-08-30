#ifndef SOFTWAREPROJECT_USERINPUT_H
#define SOFTWAREPROJECT_USERINPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "graphRepresentation.h"
#include "eigengapHeuristic.h"

void invalid_input();

void general_error();

matrix *extractData(char *fileName);

void countRowsAndCols(char *inputfile, int * rows, int * cols);

void fillData(char *inputfile, matrix * data);

void wamGoal(matrix *data, matrix **W);

void ddgGoal(matrix *data, matrix **W, matrix **D);

void lnormGoal(matrix *data, matrix **W, matrix **D, matrix **lNorm);

void jacobiGoal(matrix **A, matrix **V);

void runGoalC(char *goal, matrix *data, matrix **W, matrix **D, matrix **lNorm, matrix **A, matrix **V);

void spkGoal(matrix *data, int * k, matrix **W, matrix **D, matrix **lNorm, matrix **V);

#endif /*SOFTWAREPROJECT_USERINPUT_H*/
