#include "errorHandling.h"

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