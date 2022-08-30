#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

static void convertPythonMatrixToCMatrix(PyObject *matrix,int numOfRows,int numOfColumns,double ***cMatrix);
static PyObject* cMatrixToPythonObject(int k,int numOfColumns, double ***centroidArray);
void update_centroids(double **centroids, double **data_points, int k, int cols, int rows, int max_iter, double eps);
double **initialize_matrix(int rows, int cols);
void free_matrix(double **matrix, int n_rows);
int find_closest_centroid(double **centroids, double *vector, int k, int cols);
double distance_between_vectors(double *vec1, double *vec2, int cols);
int check_if_smaller_than_epsion(double **centroids, double **new_centroids, int k, int cols, double eps);

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

    PyresultArr = cMatrixToPythonObject(k, dimension,&centroids);
    
    free_matrix(data_points, rows);
    free_matrix(centroids, k);

    return Py_BuildValue("O", PyresultArr);
}

static PyMethodDef capiMethods[] = {
        {       "fit",
                (PyCFunction) fit,
                METH_VARARGS,
                PyDoc_STR("K means algorithm!")},
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

static void convertPythonMatrixToCMatrix(PyObject *matrix,int numOfRows,int numOfColumns,double ***cMatrix){
    int i,j;
    for(i=0; i<numOfRows;i++){
        for(j = 0; j<numOfColumns;j++){
            (*cMatrix)[i][j] = PyFloat_AsDouble(PyList_GetItem(matrix, j+(numOfColumns*i)));
        }
    }
}

static PyObject* cMatrixToPythonObject(int k,int numOfColumns, double ***centroidArray)
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

void update_centroids(double **centroids, double **data_points, int k, int cols, int rows, int max_iter, double eps) {

    int iteration_number = 0, epsilon_condition = 0;
    int i, j, temp_index_of_closest_centroid;
    double **new_centroids = initialize_matrix(k, cols);
    int *counter_data_points_per_centroid = malloc(k * sizeof(int));
    
    assert (counter_data_points_per_centroid != NULL);
    

    while (iteration_number < max_iter && epsilon_condition == 0) {
        iteration_number++;
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

int find_closest_centroid(double **centroids, double *vector, int k, int cols) {
    /*finds the row index within the centroids matrix of the closest centroid to a given vector*/
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

double distance_between_vectors(double *vec1, double *vec2, int cols) {
    /*calculates the distance between 2 vectors (only calcluates the sum of all squared deltas, without taking the kth root)*/
    double dis = 0.0;
    int i;
    for (i = 0; i < cols; i++) {
        dis = dis + ((vec1[i] - vec2[i]) * (vec1[i] - vec2[i]));
    }
    return dis;

}

int check_if_smaller_than_epsion(double **centroids, double **new_centroids, int k, int cols, double eps) {
    /*returns 1 if each of the centroids deltas are smaller than epsilon, 0 otherwise*/
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
