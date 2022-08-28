#ifndef SOFTWAREPROJECT_USERINPUT_H
#define SOFTWAREPROJECT_USERINPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "graphRepresentation.h"

void invalid_input();

void validateInput(int argc, char *argv[]);

void general_error();

matrix *extractData(char *fileName);

void wamGoal(matrix *data, matrix **W);

void ddgGoal(matrix *data, matrix **W, matrix **D);

void lnormGoal(matrix *data, matrix **W, matrix **D, matrix **lNorm);

void jacobiGoal(matrix *data, matrix **W, matrix **D, matrix **lNorm, matrix **V);

void runGoalC(char *goal, matrix *data, matrix **W, matrix **D, matrix **lNorm, matrix **V);

void runGoalPy(char *goal, matrix *data);

void runSpk(matrix *lNorm, matrix *V);

#endif /*SOFTWAREPROJECT_USERINPUT_H*/
