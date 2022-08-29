#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "spkmeans.h"

static void convertPythonMatrixToCMatrix(PyObject *mtx,int rows,int cols,matrix * data);

/*TODO:rename function*/
static void *otherGoalPy(PyObject *self, PyObject * args)
{
    char * goal;
    PyObject *pyPointArr;
    int k,rows,cols;
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL, *A = NULL, *data=NULL;

    if(!PyArg_ParseTuple(args, "iOsii", &goal, &pyPointArr,&k,&rows,&cols))
    {
        return NULL;
    }

    data = newMatrix(rows,cols);
    convertPythonMatrixToCMatrix(pyPointArr,rows,cols,data);
    runGoalC(goal, data, &W, &D, &lNorm, &A, &V);
}

static PyMethodDef capiMethods[] = {
        {       "goalPy",
                (PyCFunction) goalPy,
                METH_VARARGS,
                PyDoc_STR("Goal py")},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "spkmeanspy",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC
PyInit_spkmeanspy(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m)
    {
        return NULL;
    }
    return m;
}

static void convertPythonMatrixToCMatrix(PyObject *mtx,int rows,int cols,matrix * data){
    int i,j;
    double val;
    for(i=0; i<rows;i++){
        for(j = 0; j<cols;j++){
            val = PyFloat_AsDouble(PyList_GetItem(mtx, j+(cols*i)));
            setElement(data,i,j,val);
        }
    }
}