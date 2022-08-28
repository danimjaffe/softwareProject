#include "eigengapHeuristic.h"

/*TODO: Add general errors when creating new diagonal*/


/*Determines k based on decreasing ordered eigenvalues*/

tuple newTuple(int idx, double val){
    tuple t;
    t.idx = idx;
    t.val = val;
    return t;
}

/*Sort eigenvalues and vectors in decreasing order*/
void sortEigenvaluesAndVectors(matrix * A, matrix * V, matrix * newV){
    int rows = nRows(A),cols = nCols(A);
    int i,j;
    double val;
    tuple * diagonal = (tuple *) malloc(rows*sizeof(tuple));
    assert(diagonal != NULL);
    for (i = 1; i <= rows; i++)
    {
        val = getElement(A,i,i);
        diagonal[i-1]=newTuple(i,val);
    }

    /*Sort eigenvalues in diagonal of A in decreasing order*/

    qsort(diagonal, rows, sizeof(tuple), compareTuple); 

    /*Set decreasing ordered eigenvalues in diagonal of A*/
    for (i = 1; i <= rows; i++)
    {
        setElement(A,i,i,diagonal[i-1].val);
    }

    /* Order eigenvectors of V*/
    
    for (i = 1; i <= cols; i++)
    {
        for (j = 1; j <= rows; j++)
        {
            setElement(newV,j,i,getElement(V,j,diagonal[i-1].idx));
        }
        
    }

    /* TODO: free all alocated memory for V and for diagonal*/
    
}

int compareTuple(const void *p1, const void *p2)
{
const tuple *q1 = p1, *q2 = p2;
double val;
if((*q1).val == (*q2).val){
    return (int)((*q1).idx - (*q2).idx);
}

val = ((*q1).val - (*q2).val) > 0 ? -1 : 1;
return val;
} 

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
