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

struct vector **convert_centroids(PyObject *py_centroids, int k, int num_cords){
    struct vector *vec;
    struct cord *head_cord, *curr_cord;
    PyObject* py_cords; 
    PyObject* double_object;
    int i, j;
    struct vector **arr = malloc(k*sizeof(struct vector *));
    if(arr==NULL) return NULL;

    for(i=0; i<k; i++){
        vec = malloc(sizeof(struct vector));
        if(vec == NULL) return NULL; /*need to free memory*/
        vec->next = NULL;
        py_cords = PyList_GetItem(py_centroids, i);
        if (!PyList_Check(py_cords)) return NULL; /*need to free memory*/
        head_cord = malloc(sizeof(struct cord)); 
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

struct vector **convert_clusters(PyObject *py_clusters){
    
}

static PyObject* fit(PyObject *self, PyObject *args){
    PyObject* py_centroids;
    PyObject* py_clusters;
    PyObject* py_cords;
    struct vector **centroids;
    struct vector **clusters;
    Py_ssize_t cent_size;
    Py_ssize_t vec_size;
    int k;
    int d;

    if(!PyArgs_ParseTuple(args, "OO", &py_centroids, &py_clusters)){
        return NULL;
    }

    if (!PyList_Check(py_centroids)) return NULL;
    cent_size = PyList_Size(py_centroids);
    k = (int)cent_size;
    if(k<=0) return NULL;
    py_cords = PyList_GetItem(py_centroids, 0);
    if (!PyList_Check(py_cords)) return NULL;
    vec_size = PyList_Size(py_cords);
    d = (int)vec_size;
    centroids = convert_centroids(py_centroids, k, d);

}

static PyMethodDef kmeansMethods[] = {
    {"fit",
    (PyCFunction) fit,
    METH_VARARGS,
    PyDoc_STR("ADD DOCSTRING")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanspp",
    NULL,
    -1,
    kmeansMethods
};

PyMODINIT_FUNC PyInit_mykmeanspp(void){
    PyObject *m;
    m = PyModule_Create(&kmeansmodule);
    if(!m){
        return NULL;
    }
    return m;
}