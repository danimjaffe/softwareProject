#!/bin/bash
gcc -ansi -Wall -Wextra -Werror -pedantic-errors eigengapHeuristic.c errorHandling.c graphRepresentation.c matrix.c rotationMatrix.c spkmeans.c userInput.c -lm -o spkmeans