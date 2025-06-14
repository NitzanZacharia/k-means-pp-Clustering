#define PU_SSIZE_T_CLEAN
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

struct vector **convert_centroids(PyObject *py_centroids, int k){
    struct vector *vec;
    struct cord *head_cord, *curr_cord;
    PyObject* py_cords; 
    PyObject* double_object;
    Py_ssize_t num_cords;
    int i, j;
    struct vector **arr = malloc(k*sizeof(struct vector *));
    if(arr==NULL) return NULL;
    if (!PyList_Check(py_centroids)) return NULL;

    for(i=0; i<k; i++){
        vec = malloc(sizeof(struct vector));
        if(vec == NULL) return NULL; /*need to free memory*/
        vec->next = NULL;
        py_cords = PyList_GetItem(py_centroids, i);
        if (!PyList_Check(py_cords)) return NULL; /*need to free memory*/
        head_cord = malloc(sizeof(struct cord));
        num_cords = PyList_Size(py_cords);
        if(head_cord == NULL) return NULL; /*need to free memory*/
        curr_cord = head_cord;
        curr_cord->next = NULL;

        for(j=0; j<num_cords; j++){
            double_object = PyList_GetItem(py_cords, j);
            curr_cord->value = PyFloat_AsDouble(double_object);
            if(j+1 != num_cords){
                curr_cord->next = malloc(sizeof(struct cord));
                if(curr_cord->next == NULL) return NULL; /*need to free memory*/
                curr_cord = curr_cord->next;
            }
            else curr_cord->next = NULL;
        }
        vec->cords = head_cord;
        arr[i] = vec;
    }

    return arr;
}

struct vector **convert_clusters(PyObject *py_clusters, int k){
    
}

static PyObject* fit(PyObject *self, PyObject *args){
    PyObject* py_centroids;
    PyObject* py_clusters;
    int k;
    int d;
    struct vector **centroids;
    struct vector **clusters;

    if(!PyArgs_ParseTuple(args, "OOii", &py_centroids, &py_clusters, &k, &d)){
        return NULL;
    }

    centroids = convert_centroids(py_centroids, k);

}