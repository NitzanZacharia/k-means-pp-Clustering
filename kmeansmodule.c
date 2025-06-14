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

/*use this to default when crashing*/
void exit_func(){
    printf("An Error Has Occured\n");
    exit(1);
}

int is_digit(char c) {
    if(c >= '0' && c <= '9') return 0;
    return 1;
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

int e_convergence(struct vector **prev_centr, struct vector **curr_centr, int k){
    double curr_dist;
    int i;
    for(i = 0; i < k; i++){
        curr_dist = dist(prev_centr[i]->cords, curr_centr[i]->cords);
        if(curr_dist>=0.001){
            return 0;
        }
    }
    return 1;
}

int min_dist(struct vector *vec, struct vector **arr, int k){
    double min_dist = 1e100;
    int min_idx;
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

struct vector **init_centroids(struct vector *head_vec, int k){
    struct vector **arr = malloc(k*sizeof(struct vector *));
    struct vector *new_vec;
    int i, j;
    if(arr == NULL){
        exit_func();
    }
    for (i = 0; i < k; i++) {
        new_vec = deep_copy_vector(head_vec);
        if (new_vec == NULL) {
            for ( j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            exit_func();
        }
        arr[i] = new_vec;
        head_vec = head_vec->next;
    }
    return arr;
}

int validate_input(const char *k, const char *iter, int n){ 
    char *kerr;
    char *itererr;
    double knum = strtod(k, &kerr);
    double iternum = strtod(iter, &itererr);
    if( floor(knum)!= knum || knum<2 || knum>=n || (*kerr != '\0')){
        printf("Incorrect number of clusters!\n");
        return 0;
    }
    if(floor(iternum) != iternum || iternum<2 || iternum >=1000 || (*itererr != '\0')){
        printf("Incorrect maximum iteration!\n");
        return 0;
    }
    return 1;
}

void print_rep(struct vector **arr, int k){
    struct vector *head_vec;
    struct cord *curr_cord;
    int i;
    int j;
    for (i = 0; i < k; i++) {
        head_vec = arr[i];
        curr_cord = head_vec->cords;
        while (curr_cord != NULL)
        {
            printf("%.4f",curr_cord->value);
            if (curr_cord->next != NULL)
            {
                printf(",");
            }
            curr_cord = curr_cord->next;
        
        }   
        printf("\n");  
    }
    
    for (j = 0; j < k; j++) {
        free_head_vec(arr[j]); 
    }
    free(arr); 
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
        exit_func();
    }
    
    for (i = 0; i < k; i++) {
        updated_cnt_arr[i] = deep_copy_vector(cnt_arr[i]);
        if (updated_cnt_arr[i] == NULL) {
            
            for (j = 0; j < i; j++) {
                free_head_vec(updated_cnt_arr[j]);
            }
            free(updated_cnt_arr);
                exit_func();
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
            exit_func();
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
        exit_func();
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
                exit_func(); 
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

int main(int argc, char **argv)
{
    int k, iter, i, ind, ind2, a;
    char *arg2;
    struct vector *head_vec, *curr_vec; /* *next_vec */
    struct cord *head_cord, *curr_cord; /* *next_cord */
    struct vector *tmp;
    int rows = 0;
    double n;
    char c;
    int flag = 1;
    int d = 0;
    int res;
    struct vector **centroids;
    struct vector **clusters;
    struct vector **new_centroids;

    head_cord = malloc(sizeof(struct cord));
    if(head_cord == NULL) exit_func();
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if(head_vec == NULL) exit_func();
    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            curr_cord->value = n;
            if(flag)
            {
            d++;
            flag = 0;
            }
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if(curr_vec->next == NULL) exit_func();
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if(head_cord == NULL) exit_func();
            curr_cord = head_cord;
            curr_cord->next = NULL;
            rows++;
            continue;
        }

        curr_cord->value = n;
        if(flag)
        {
            d++;
        }
        curr_cord->next = malloc(sizeof(struct cord));
        if(curr_cord->next == NULL) exit_func();
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }

    /*validate args*/
    if(argc < 2 || argc > 3) exit_func();
    if(argc == 2){arg2 = "400";}
    else{arg2 = argv[2];}
    res = validate_input(argv[1], arg2, rows);
    if(res == 0){return 1;}
    k = atoi(argv[1]);
    iter = atoi(arg2);
    
    /* deleting last empty vector*/
    tmp = head_vec;
 
    for( i=0; i<rows-1; i++) tmp = tmp->next;
    tmp->next = NULL;

    /*init clusters and centroids*/
    new_centroids = init_centroids(head_vec, k);
    clusters = malloc(k*sizeof(struct vector *));
    if(clusters == NULL){
        free_head_vec(head_vec);
        exit_func();
    }
    clusters[0] = head_vec;
    
    for( a=1; a<k; a++){
        clusters[a] = NULL;
    }
    
    /*cluster the datapoints*/
    for(ind=0; ind<iter; ind++){
        centroids = new_centroids;
        clusters = re_kluster(centroids, clusters, k, d);
        new_centroids = new_cent(centroids, clusters, k, d);
        if(e_convergence(centroids, new_centroids, k) == 1){
            break;
        }
    }
    /*free all datapoints and our non-centroids arrays*/
    
    for(ind2=0; ind2<k; ind2++){
        free_head_vec(clusters[ind2]);
        free_head_vec(centroids[ind2]);
    }
    free(clusters);
    free(centroids);
    print_rep(new_centroids, k);
    return 0;
}

