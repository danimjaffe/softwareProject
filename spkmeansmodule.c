#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "spkmeans.h"

static void convertPythonMatrixToCMatrix(PyObject *mtx,int rows,int cols,matrix * data);
static PyObject* cMatrixToPythonObject(int rows,int cols, matrix * mtx);

/*TODO:rename function*/
static void runGoalPy(PyObject *self, PyObject * args)
{
    char * goal;
    PyObject *pyPointArr,*PyresultArr;
    int k,rows,cols;
    matrix *W = NULL, *D = NULL, *lNorm = NULL, *V = NULL, *A = NULL, *data=NULL;

    if(!PyArg_ParseTuple(args, "sOiii", &goal, &pyPointArr,&k,&rows,&cols))
    {
        return NULL;
    }

    data = newMatrix(rows,cols);
    convertPythonMatrixToCMatrix(pyPointArr,rows,cols,data);

    if (strcmp(goal, "spk") == 0)
    {
        spkGoal(data, &k, &W, &D, &lNorm, &V);
        PyresultArr = cMatrixToPythonObject(rows,k,V);
        return PyresultArr;
    }

    runGoalC(goal, data, &W, &D, &lNorm, &A, &V);
    return NULL;
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
        "spkmeansc",
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
            setElement(data,i+1,j+1,val);
        }
    }
}

static PyObject* cMatrixToPythonObject(int rows,int cols, matrix * mtx)
{
    int i,j;
    double val;
    PyObject *list = PyList_New(rows*cols);
    for(i=0; i<cols; i++){
        for(j=0;j<cols;j++){
            val = getElement(mtx,i+1,j+1);
            PyList_SetItem(list, (i*cols)+j,PyFloat_FromDouble(val));
        }
    }
    return list;
}