#!/bin/bash
gcc eigengapHeuristic.c graphRepresentation.c matrix.c eigengapHeuristic.c spkmeansmodule.c userInput.c
gcc -ansi -Wall -Wextra -Werror -pedantic-errors spkmeans.c -lm -o spkmeans