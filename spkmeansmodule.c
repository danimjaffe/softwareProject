#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "spkmeans.h"

static PyObject* cMatrixToPythonObject(matrix * mtx);
static void convertPythonMatrixToCMatrix(PyObject *matrix,int numOfRows,int numOfColumns,double ***cMatrix);
static PyObject* cMatrixToPythonObject1(int k,int numOfColumns, double ***centroidArray);
void update_centroids(double **centroids, double **data_points, int k, int cols, int rows, int max_iter, double eps);
double **initialize_matrix(int rows, int cols);
void free_matrix(double **matrix, int n_rows);
int find_closest_centroid(double **centroids, double *vector, int k, int cols);
double distance_between_vectors(double *vec1, double *vec2, int cols);
int check_if_smaller_than_epsion(double **centroids, double **new_centroids, int k, int cols, double eps);

/*C function to run python goal*/
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
        PyresultArr = cMatrixToPythonObject(V);
    }

    else{
        runGoalC(goal, data, &W, &D, &lNorm, &A, &V);
        PyresultArr = PyList_New(0);
    }

    deleteMatrix(W);
    deleteMatrix(D);
    deleteMatrix(lNorm);
    deleteMatrix(A);
    deleteMatrix(V);
    deleteMatrix(data);
    return PyresultArr;
}

/*C function to run fit as part of kmeans++ algorithm*/
static PyObject *fit(PyObject *self, PyObject * args)
{
    int k, max_iter, rows, dimension;
    double **data_points, **centroids;
    double eps;
    PyObject *PycentArr;
    PyObject *PypointArr;
    PyObject *PyresultArr;

    if(!PyArg_ParseTuple(args, "iidiiOO", &k, &max_iter,&eps, &dimension, &rows, &PycentArr, &PypointArr))
    {
        return NULL;
    }

    data_points=initialize_matrix(rows,dimension);
    centroids=initialize_matrix(k,dimension);

    convertPythonMatrixToCMatrix(PypointArr,rows,dimension, &data_points);
    convertPythonMatrixToCMatrix(PycentArr,k,dimension, &centroids);

    update_centroids(centroids, data_points, k, dimension, rows, max_iter, eps);

    PyresultArr = cMatrixToPythonObject1(k, dimension,&centroids);

    free_matrix(data_points, rows);
    free_matrix(centroids, k);

    return Py_BuildValue("O", PyresultArr);
}

static PyMethodDef capiMethods[] = {
        {       "goalPy",
                (PyCFunction) runGoalPy,
                METH_VARARGS,
                PyDoc_STR("Goal py")},
        {       "fit",
        (PyCFunction) fit,
        METH_VARARGS,
        PyDoc_STR("fit")},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "myspkmeans",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC
PyInit_myspkmeans(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m)
    {
        return NULL;
    }
    return m;
}

/* Convert C matrix to python matrix*/
static PyObject* cMatrixToPythonObject(matrix * mtx)
{
    int i,j;
    int rows = nRows(mtx);
    int cols = nCols(mtx);
    double val;
    PyObject *list,*row;
    list = PyList_New(rows);
    for(i=1; i<=rows; i++){
        row = PyList_New(cols);
        for(j=1;j<=cols;j++){
            val = getElement(mtx,i,j);
            PyList_SetItem(row, j-1,PyFloat_FromDouble(val));
        }
        PyList_SetItem(list,i-1,row);
    }
    return list;

}

/* Convert python matrix to python C matrix*/
static void convertPythonMatrixToCMatrix(PyObject *matrix,int numOfRows,int numOfColumns,double ***cMatrix){
    int i,j;
    for(i=0; i<numOfRows;i++){
        for(j = 0; j<numOfColumns;j++){
            (*cMatrix)[i][j] = PyFloat_AsDouble(PyList_GetItem(matrix, j+(numOfColumns*i)));
        }
    }
}

/* Convert C matrix to python list*/
static PyObject* cMatrixToPythonObject1(int k,int numOfColumns, double ***centroidArray)
{
    int i,j;
    PyObject *list = PyList_New(k*numOfColumns);
    for(i=0; i<k; i++){
        for(j=0;j<numOfColumns;j++){
            PyList_SetItem(list, (i*numOfColumns)+j,PyFloat_FromDouble((*centroidArray)[i][j]));
        }
    }
    return list;
}

/* Update centroids based on kmenas++ logic*/
void update_centroids(double **centroids, double **data_points, int k, int cols, int rows, int max_iter, double eps) {

    int iteration_number = 0, epsilon_condition = 0;
    int i, j, temp_index_of_closest_centroid;
    double **new_centroids = initialize_matrix(k, cols);
    int *counter_data_points_per_centroid = malloc(k * sizeof(int));

    assert (counter_data_points_per_centroid != NULL);


    while (iteration_number < max_iter && epsilon_condition == 0) {
        iteration_number++;
        /* Reset counter_data_points_per_centroid and new_centroids */
        for (i = 0; i < k; i++) {
            counter_data_points_per_centroid[i] = 0;
            for (j = 0; j < cols; j++) {
                new_centroids[i][j] = 0.0;
            }

        }
        for (i = 0; i < rows; i++) {
            temp_index_of_closest_centroid = find_closest_centroid(centroids, data_points[i], k, cols);
            counter_data_points_per_centroid[temp_index_of_closest_centroid]++;
            for (j = 0; j < cols; j++) {
                new_centroids[temp_index_of_closest_centroid][j] += data_points[i][j];
            }
        }
        for (i = 0; i < k; i++) {
            for (j = 0; j < cols; j++) {
                new_centroids[i][j] = new_centroids[i][j] / counter_data_points_per_centroid[i];
            }
        }

        epsilon_condition = check_if_smaller_than_epsion(centroids, new_centroids, k, cols, eps);
        if (epsilon_condition == 0) {
            for (i = 0; i < k; i++) {
                for (j = 0; j < cols; j++) {
                    centroids[i][j] = new_centroids[i][j];
                }

            }

        }

    }
    free(counter_data_points_per_centroid);
    free_matrix(new_centroids, k);
}

/* initialize matrix*/
double **initialize_matrix(int rows, int cols) {

    double **a;
    int i;
    a = calloc(rows, sizeof(double *));
    assert (a != NULL);

    for (i = 0; i < rows; i++){
        a[i] = calloc(cols, sizeof(double));
        assert (a[i] != NULL);
    }

    return a;
}

void free_matrix(double **matrix, int n_rows) {
    int i = 0;
    for (; i < n_rows; i++)
        free(matrix[i]);
    free(matrix);
}

/*finds the row index within the centroids matrix of the closest centroid to a given vector*/
int find_closest_centroid(double **centroids, double *vector, int k, int cols) {
    double tempdistance;
    double min_distance = distance_between_vectors(centroids[0], vector, cols);
    int min_distance_index = 0, i;
    for (i = 1; i < k; i++) {
        tempdistance = distance_between_vectors(centroids[i], vector, cols);
        if (tempdistance < min_distance) {
            min_distance = tempdistance;
            min_distance_index = i;
        }

    }
    return min_distance_index;

}

/*calculates the distance between 2 vectors (only calcluates the sum of all squared deltas, without taking the kth root)*/
double distance_between_vectors(double *vec1, double *vec2, int cols) {
    double dis = 0.0;
    int i;
    for (i = 0; i < cols; i++) {
        dis = dis + ((vec1[i] - vec2[i]) * (vec1[i] - vec2[i]));
    }
    return dis;

}

/*returns 1 if each of the centroids deltas are smaller than epsilon, 0 otherwise*/
int check_if_smaller_than_epsion(double **centroids, double **new_centroids, int k, int cols, double eps) {
    int i, epsilon_dis_counter = 0;
    double dis;
    for (i = 0; i < k; i++) {
        dis = distance_between_vectors(centroids[i], new_centroids[i], cols);
        dis = sqrt(dis);
        if (dis < eps) {
            epsilon_dis_counter++;
        }
    }
    if (epsilon_dis_counter == k) {
        return 1;
    }

    return 0;
}