#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "spkmeans.h"

static PyObject* cMatrixToPythonObject(int rows,int cols, matrix * mtx);

/*TODO:rename function*/
static PyObject* runGoalPy(PyObject *self, PyObject * args)
{
    char * goal, * fileName;
    PyObject *PyresultArr;
    int k;
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL, *A = NULL, *data=NULL;

    if(!PyArg_ParseTuple(args, "iss",&k,&goal,&fileName))
    {
        return NULL;
    }

    data = extractData(fileName);

    if (strcmp(goal, "spk") == 0)
    {
        spkGoal(data, &k, &W, &D, &lNorm, &V);
        PyresultArr = cMatrixToPythonObject(nRows(V),nCols(V),V);
        return PyresultArr;
    }

    runGoalC(goal, data, &W, &D, &lNorm, &A, &V);
    return PyList_New(0);
}


static PyMethodDef capiMethods[] = {
        {       "goalPy",
                (PyCFunction) runGoalPy,
                METH_VARARGS,
                PyDoc_STR("Goal py")},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m)
    {
        return NULL;
    }
    return m;
}

static PyObject* cMatrixToPythonObject(int rows,int cols, matrix * mtx)
{
    int i,j;
    double val;
    PyObject *list = PyList_New(rows*cols);
    for(i=0; i<rows; i++){
        for(j=0;j<cols;j++){
            val = getElement(mtx,i+1,j+1);
            PyList_SetItem(list, (i*cols)+j,PyFloat_FromDouble(val));
        }
    }
    return list;
}