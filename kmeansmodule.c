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

int free_head_vec(struct vector *head_vec){
    struct cord *curr_cord;
    struct cord *tmp_cord;
    struct vector *tmp_vec;
    while(head_vec != NULL){
        curr_cord = head_vec->cords;
        while(curr_cord != NULL){
            tmp_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(tmp_cord);
        }
        tmp_vec = head_vec;
        head_vec = head_vec->next;
        free(tmp_vec);
    }
    return 0;
}

struct vector **convert_centroids(PyObject *py_centroids, int k, int num_cords){
    struct vector *vec;
    struct cord *head_cord, *curr_cord;
    PyObject* py_cords; 
    PyObject* double_object;
    int i, j, item;
    struct vector **arr = malloc(k*sizeof(struct vector *));
    if(arr==NULL) return NULL;

    for(i=0; i<k; i++){
        vec = malloc(sizeof(struct vector));
        if(vec == NULL){
            for(item=0; item<i; item++){
                free_head_vec(arr[item]);
            }
            free(arr);
            return NULL;
        } 
        arr[i] = vec;
        vec->next = NULL;
        vec->cords = NULL;
        py_cords = PyList_GetItem(py_centroids, i);
        if (!PyList_Check(py_cords)){
            for(item=0; item<i+1; item++){
                free_head_vec(arr[item]);
            }
            free(arr);
            return NULL;
        }
        head_cord = malloc(sizeof(struct cord)); 
        if(head_cord == NULL){
            for(item=0; item<i+1; item++){
                free_head_vec(arr[item]);
            }
            free(arr);
            return NULL;
        }  
        curr_cord = head_cord;
        curr_cord->next = NULL;
        vec->cords = head_cord;
        for(j=0; j<num_cords; j++){
            double_object = PyList_GetItem(py_cords, j);
            curr_cord->value = PyFloat_AsDouble(double_object);
            if(j+1 != num_cords){
                curr_cord->next = malloc(sizeof(struct cord));
                if(curr_cord->next == NULL){
                    for(item=0; item<i+1; item++){
                        free_head_vec(arr[item]);
                    }
                    free(arr);
                    return NULL;
                }
                curr_cord = curr_cord->next;
            }
            else curr_cord->next = NULL;
        }
    }
    return arr;
}

struct vector **convert_clusters(PyObject *py_clusters, int d, int k){
    Py_ssize_t lst_size;
    PyObject *py_cords;
    PyObject *double_object;
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    struct vector **arr;
    int n, i, j;
    arr = malloc(k*sizeof(struct vector *));
    if(arr==NULL) return NULL;
    if (!PyList_Check(py_clusters)) return NULL;
    lst_size = PyList_Size(py_clusters);
    n = (int) lst_size;
    if(n<=0 || d<=0) return NULL;
    head_vec = malloc(sizeof(struct vector));
    if(head_vec == NULL) return NULL;
    curr_vec = head_vec;
    head_cord = malloc(sizeof(struct cord));
    if(head_cord == NULL){
        free(head_vec);
        return NULL;
    }
    
    for(i=0; i<n; i++){
        curr_vec->cords = head_cord;
        curr_cord = head_cord;
        curr_cord->next = NULL;
        py_cords = PyList_GetItem(py_clusters, i);
        if (!PyList_Check(py_cords)){
            free_head_vec(head_vec);
            return NULL;
        }
        for(j=0; j<d; j++){
            double_object = PyList_GetItem(py_cords, j);
            curr_cord->value = PyFloat_AsDouble(double_object);
            if(j+1==d){
                curr_cord->next = NULL;
            }
            else{
                curr_cord->next = malloc(sizeof(struct cord));
                if(curr_cord->next == NULL){
                    free_head_vec(head_vec);
                    return NULL;
                }
                curr_cord = curr_cord->next;
            }
        }
        if(i+1<n){
            curr_vec->next = malloc(sizeof(struct vector));
            if(curr_vec->next == NULL){
                free_head_vec(head_vec);
                return NULL;
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            curr_vec->cords = NULL;
            head_cord = malloc(sizeof(struct cord));
            if(head_cord==NULL){
                free_head_vec(head_vec);
                return NULL;
            }
        }
    }
    arr[0] = head_vec;
    for(i=1; i<k; i++){
        arr[i] = NULL;
    }
    return arr;
} 

double dist(struct cord *p, struct cord *q)
{   
    double sum=0.0;
    double temp;
    
    while(p != NULL&& q != NULL){
       temp =  p->value - q->value;
       sum += pow(temp,2);
       p = p->next; 
       q = q->next;

    }
    return sqrt(sum); 
}

int e_convergence(struct vector **prev_centr, struct vector **curr_centr, int k, double e){
    double curr_dist;
    int i;
    for(i = 0; i < k; i++){
        curr_dist = dist(prev_centr[i]->cords, curr_centr[i]->cords);
        if(curr_dist>=e){
            return 0;
        }
    }
    return 1;
}

int min_dist(struct vector *vec, struct vector **arr, int k){
    double min_dist = 1e100;
    int min_idx = -1;
    double curr_dist;
    struct cord *cor = vec->cords;
    int i;
    for(i=0; i<k; i++){
        curr_dist = dist(cor, arr[i]->cords);
        if(curr_dist<min_dist) {
            min_dist = curr_dist;
            min_idx = i;
        }
    }
    return min_idx;
}

struct vector *deep_copy_vector(struct vector *vec)
{
    struct cord *curr_cord, *new_cord, *prev_cord;
    struct vector *new_vec = malloc(sizeof(struct vector));
    if (new_vec == NULL) {
        return NULL; 
    }
    new_vec->cords = NULL;
    new_vec->next = NULL;

    curr_cord = vec->cords;
    new_cord = NULL;
    prev_cord = NULL;
    while(curr_cord != NULL)
    {
        if (new_vec->cords == NULL) {
            new_vec->cords = malloc(sizeof(struct cord));
            if (new_vec->cords == NULL) {
                free(new_vec);
                return NULL; 
            }
            new_cord = new_vec->cords;
        } else{
            new_cord = malloc(sizeof(struct cord));
            if (new_cord == NULL) {
                free_head_vec(new_vec);
                return NULL; 
            }
            prev_cord->next = new_cord;
        }
        new_cord->value = curr_cord->value;
        new_cord->next = NULL;
        prev_cord = new_cord;
        curr_cord = curr_cord->next;
    }
    return new_vec;
} 

struct vector **new_cent(struct vector **cnt_arr, struct vector **klst_arr,  int k, int d){
    int i;
    int j;
    int b;
    int a;
    int ind, cnt;
    struct cord *updated_cnt;
    struct vector *curr_vec;
    struct vector **updated_cnt_arr = malloc(k * sizeof(struct vector *));
    if (updated_cnt_arr == NULL) {
        return NULL;
    }
    
    for (i = 0; i < k; i++) {
        updated_cnt_arr[i] = deep_copy_vector(cnt_arr[i]);
        if (updated_cnt_arr[i] == NULL) {
            
            for (j = 0; j < i; j++) {
                free_head_vec(updated_cnt_arr[j]);
            }
            free(updated_cnt_arr);
            return NULL;
        }
    }
   
    for ( j = 0; j < k; j++)
    {
        
        double *arr_sum = calloc(d, sizeof(double)); /* get as args instead of creating a new one each time?*/
        if (arr_sum==NULL){
            
            for (a = 0; a < k; a++) {
                free_head_vec(updated_cnt_arr[a]);  
            }
            free(updated_cnt_arr);
            return NULL;
        }
        
        cnt = 0;
        curr_vec = klst_arr[j];
        while (curr_vec != NULL) {
            struct cord *curr_cord = curr_vec->cords;
            
            for ( b = 0; b < d; b++) {
                arr_sum[b] += curr_cord -> value;
                curr_cord = curr_cord-> next; 
            }

           cnt++;
           curr_vec = curr_vec->next;

        }
        
        updated_cnt = updated_cnt_arr[j]->cords;
        
        for (ind = 0; ind < d; ind++) {
            if (cnt>0)
            {
                updated_cnt->value = arr_sum[ind] / cnt;
            
            }
           updated_cnt = updated_cnt->next;
        }

        free(arr_sum);
    }
    return updated_cnt_arr;
}
    
struct vector **re_kluster(struct vector **cent_arr, struct vector **org_klst_arr,  int k, int d){
    int m;
    int i;
    int j;
    int a;
    struct vector *curr_vec, *new_vec, *head_vec;
    struct vector **klst_arr = malloc(k*sizeof(struct vector *));
    
    (void)d; /*delete?*/
    
    if(klst_arr == NULL){
        free(org_klst_arr);
        return NULL;
    }
    
    for (a = 0; a < k; a++) {
        klst_arr[a] = NULL;
    }

    
    for (i = 0; i < k; i++) {
        head_vec = org_klst_arr[i];
        curr_vec = head_vec;
        while (curr_vec!=NULL)
        { 
            new_vec = deep_copy_vector(curr_vec);
            if (new_vec == NULL) {
                for (j = 0; j < i; j++) {
                    free(klst_arr[j]);
                }
                free(klst_arr);
                free(org_klst_arr);
                return NULL;
            }

            /* min_dist(struct vector *vec, struct vector **arr, int k) - returns the index of the kluster that the vector needs to be reassigned to*/
            m = min_dist(new_vec, cent_arr,k);
            if(klst_arr[m] == NULL){
                klst_arr[m] = new_vec;

            }
            else{
                new_vec->next = klst_arr[m];
                klst_arr[m] = new_vec;

            }
            curr_vec = curr_vec->next;
        }
        free_head_vec(head_vec);
    }
    free(org_klst_arr);
    return klst_arr;
}


struct vector **kmeans_algo(struct vector **init_cent, struct vector **init_clust, int k, int d, int iter, double e){
    struct vector **centroids;
    struct vector **new_centroids;
    struct vector **clusters;
    int i, j;
    int is_error = 0;

    centroids = malloc(k*sizeof(struct vector));
    if(centroids == NULL) {
        is_error=1;
        return NULL;
    }
    for(i=0; i<k; i++){
        centroids[i] = NULL;
    }
    new_centroids = init_cent;
    clusters = init_clust;

    for(i=0; i<iter; i++){
        for(j=0; j<k; j++){
            free_head_vec(centroids[j]);
        }
        free(centroids);
        centroids = new_centroids;
        clusters = re_kluster(centroids, clusters, k, d);
        if(clusters == NULL){
            is_error = 1;
            goto free_mem;
        }
        new_centroids = new_cent(centroids, clusters, k, d);
        if(new_centroids == NULL){
            is_error=1;
            goto free_mem;
        }
        if(e_convergence(centroids, new_centroids, k, e) == 1){
            break;
        }
    }

free_mem:
    /*free all memory and exit*/
    for(i=0; i<k; i++){
        if(centroids != NULL) free_head_vec(centroids[i]);
        if(clusters != NULL) free_head_vec(clusters[i]);
    }
    if(centroids != NULL) free(centroids);
    if(clusters != NULL) free(clusters);

    if(is_error==1) return NULL;
    return new_centroids;
}

PyObject* convertto_pyobject(struct vector **centroids, int k, int d){
    PyObject* py_vector;
    PyObject* py_list;
    struct vector *vec;
    struct cord *cor;
    int i, j;

    py_list = PyList_New(k);
    if(!py_list) return NULL;
    
    for(i=0; i<k; i++){
        py_vector = PyList_New(d);
        if(!py_vector){
            for(j=0; j<i; j++){
                Py_XDECREF(PyList_GetItem(py_list, j));
            }
            Py_XDECREF(py_list);
            return NULL;
        }
        vec = centroids[i];
        cor = vec->cords;
        for(j=0; j<d; j++){
            PyList_SET_ITEM(py_vector, j, PyFloat_FromDouble(cor->value));
            cor = cor->next;
        }
        PyList_SET_ITEM(py_list, i, py_vector);
        free_head_vec(vec);
    }
    free(centroids);
    return py_list;
}

static PyObject* fit(PyObject *self, PyObject *args){
    PyObject* py_centroids;
    PyObject* py_clusters;
    PyObject* py_cords;
    PyObject* py_result;
    struct vector **centroids;
    struct vector **clusters;
    struct vector **final_cent;
    Py_ssize_t cent_size;
    Py_ssize_t vec_size;
    int k, d, i;
    double e;
    int iter;

    if(!PyArg_ParseTuple(args, "OOid", &py_centroids, &py_clusters, &iter, &e)){
        PyErr_SetString(PyExc_ValueError, "Invalid args!");
        return NULL;
    }

    if (!PyList_Check(py_centroids)){
        PyErr_SetString(PyExc_ValueError, "Invalid centroids!");
        return NULL;
    }
    cent_size = PyList_Size(py_centroids);
    k = (int)cent_size;
    if(k<=0){
        PyErr_SetString(PyExc_ValueError, "Invalid k!");
        return NULL;
    }
    py_cords = PyList_GetItem(py_centroids, 0);
    if (!PyList_Check(py_cords)){
        PyErr_SetString(PyExc_ValueError, "Invalid centroids!");
        return NULL;
    }
    vec_size = PyList_Size(py_cords);
    d = (int)vec_size;
    centroids = convert_centroids(py_centroids, k, d);
    if(centroids == NULL){
        PyErr_SetString(PyExc_ValueError, "Invalid centroids!");
        return NULL;
    }
    clusters = convert_clusters(py_clusters, d, k);
    if(clusters == NULL){
        for(i=0; i<k; i++){
            free_head_vec(centroids[i]);
        }
        free(centroids);
        PyErr_SetString(PyExc_ValueError, "Invalid datapoints!");
        return NULL;
    }

    final_cent = kmeans_algo(centroids, clusters, k, d, iter, e);
    if(final_cent == NULL) {
        PyErr_SetString(PyExc_ValueError, "Something went wrong!");
        return NULL;
    }
    py_result = convertto_pyobject(final_cent, k, d);
    if(py_result == NULL){
        PyErr_SetString(PyExc_ValueError, "Something went wrong!");
        return NULL;
    }
    return (py_result);
}

static PyMethodDef kmeansMethods[] = {
    {"fit",
    (PyCFunction) fit,
    METH_VARARGS,
    PyDoc_STR("A kmeans algorithm that sorts into k clusters, expected: fit(centroids: list of k initial centroids (list of lists of floats) , points: list of data points (list of lists of floats), iter: maximum number of iterations (int), epsilon: convergence threshold (float))")}, 
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