#include <stdio.h>
#include <stdlib.h>

#include "nj_kheap.h"

void min_heapify(dmatrix* D, int index);
void heapify_qmatrix(dmatrix* D);

void min_heapify_kheap(dmatrix* D, int index);
void heapify_qmatrix_kheap(dmatrix* D);

void dmatrix_update_kheap(dmatrix *D, otu_t data);

void get_otu_qmatrix_kheap(dmatrix *D, otu_t* out, int* out_size);

void merge(float* org, int start, int middle, int end, float* temp){
    int i, j, k;
    i = start;
    j = middle + 1;
    k = 0;

    while (i <= middle && j <= end){
        if (org[i] < org[j]){
            temp[k] = org[i];
            i++;
        }
        else{
            temp[k] = org[j];
            j++;
        }

        k++;
    }

    while ( i <= middle ){
        temp[k] = org[i];
        k++;
        i++;
    }

    while ( j <= end ){
        temp[k] = org[j];
        k++;
        j++;
    }

    for (i = start; i < end+1; i++)
    {
        org[i] = temp[i-start];
    }
}

void split(float* org, int start, int end, float *temp){
    if(start>=end)
        return;

    int middle = (start+end)/2;

    split(org, start, middle, temp);
    split(org, middle+1, end, temp);

    merge(org, start, middle, end, temp);
}

int merge_sort_nodes(dmatrix* D, int start1, int end1, int start2, int end2){

    float* temp_vector = (float*) calloc(sizeof(float), 2*D->k);
    float* temp_vector_aux = (float*) calloc(sizeof(float), 2*D->k);
    int i, aux;

    if(temp_vector == NULL)
        return -1;

    i = start1;
    aux = 0;

    while (i < end2){
        if(i >= end1 && i < start2)
            i = start2;

        temp_vector[aux] = D->qvalues->vector[i];

        i++;
        aux++;
    }
    split(temp_vector, 0, end2+end1-start1-start2-1, temp_vector_aux);
    
    i = start1;
    aux = 0;

    while (i < end2){
        if(i >= end1 && i < start2)
            i = start2;

        D->qvalues->vector[i] = temp_vector[aux];

        i++;
        aux++;
    }

    free(temp_vector);
    free(temp_vector_aux);
    return 0;
}

void nj_kheap(dmatrix *D){
    int accumulate, collect;
    int *collected =  calloc(sizeof(int), D->N);
    otu_t* otus_k = calloc(sizeof(otu_t), D->k);

    collect = D->N*D->p_value;
    if(collect == 0) collect = 1;

    otu_t otus[collect];
    int out_size;

    while(D->N >= 3){

        for (int i = 0; i < D->N; i++){
            collected[i] = 0;
        }
        
        calculate_q_matrix(D);
        heapify_qmatrix_kheap(D);

        accumulate = 0;
        // printf("%d %.2f %d\n", D->N, D->p_value, collect);
        while(accumulate < collect){
            get_otu_qmatrix_kheap(D, otus_k, &out_size);
            
            for (int i = 0; i < out_size; i++)
            {
                if( collected[otus_k[i].i] == 0 && collected[otus_k[i].j] == 0){
                    collected[otus_k[i].i] = 1;
                    collected[otus_k[i].j] = 1;
                    otus[accumulate] = otus_k[i];

                    accumulate++;
                }
            }
            
        }
    
        for (int i = 0; i < collect; i++){
            dmatrix_update_kheap(D, otus[i]);
            
            for (int k = 0; k < collect; k++){
                if(otus[k].i == D->N){
                    otus[k].i = otus[i].i;
                }
                if(otus[k].j == D->N){
                    otus[k].j = otus[i].j;
                }
            }
        }
                
        // D->N -= collect;
        collect = D->N*D->p_value;
        if(collect == 0) collect = 1;
    }
    free(collected);
    free(otus_k);
}

void dmatrix_update_kheap(dmatrix *D, otu_t data){
    int k;
    // abaixo, gambiarra, intuicao sem prova nem definicao
    const float n_minus_2 = D->N - 2;
    const float d_ij = get_dmatrix(D, data.i, data.j);

    float dif_sums_ij, d_ui, d_uj, sum_u, sub;
    dif_sums_ij = get_sum(D, data.i) - get_sum(D, data.j);
    dif_sums_ij = dif_sums_ij/n_minus_2;
    dif_sums_ij = dif_sums_ij/2;

    d_ui = d_ij/2 + dif_sums_ij;
    d_uj = d_ij - d_ui;

    sum_u = get_sum(D, data.j);
    sub = (n_minus_2)*d_uj + d_ij;
    set_sum(D, data.j, sum_u-sub); 

    for(k = 0; k < D->N; ++k){
        if( (k == data.i) || (k == data.j))
            continue;
        
        float d_uk = distance_uk_heap(D, data.i, data.j, k);
        
        float d_ik = get_dmatrix(D, data.i, k);
        float d_jk = get_dmatrix(D, data.j, k);
        
        float sum_k = get_sum(D, k);
        sum_k += d_uk - d_ik - d_jk;
        set_sum(D, k, sum_k);

        set_dmatrix(D, data.j, k, d_uk);   
    }

    dmatrix_resize(D, data.i);
}

void heapify_qmatrix_kheap(dmatrix* D){
    int size = D->qvalues->heap_size;

    for (int i = size/(2*D->k); i >= 0; i--)
    {
        min_heapify_kheap(D, i);
    }
}

void min_heapify_kheap(dmatrix* D, int index){
    int l_node, r_node, bigger, smaller;
    l_node = 2*index + 1;
    r_node = 2*index + 2;

    float temp;
    int temp_int;

    smaller = l_node;
    bigger = r_node;

    if (l_node*D->k >= D->qvalues->heap_size)
        return;
    
    if(r_node*D->k < D->qvalues->heap_size){

        if( r_node*D->k + D->k <= D->qvalues->heap_size){
            
            if(D->qvalues->vector[ (l_node+1)*D->k -1 ] > D->qvalues->vector[ (r_node+1)*D->k -1 ])
                bigger = l_node;

            merge_sort_nodes(D, l_node*D->k, l_node*D->k + D->k - 1, r_node*D->k, r_node*D->k + D->k - 1);
        }else
            merge_sort_nodes(D, l_node*D->k, l_node*D->k + D->k - 1, r_node*D->k, D->qvalues->heap_size);

    }else{
        merge_sort_nodes(D, index*D->k, index*D->k + D->k - 1, l_node*D->k, D->qvalues->heap_size);
        return;
    }

    if (bigger == l_node)
        for (int i = 0; i < D->k; i++){
            temp = D->qvalues->vector[l_node*D->k + i];
            D->qvalues->vector[l_node*D->k + i] = D->qvalues->vector[r_node*D->k + i];
            D->qvalues->vector[r_node*D->k + i] = temp;
        }
    
    if (bigger == l_node)
        smaller = r_node;

    merge_sort_nodes(D, index*D->k, index*D->k + D->k - 1, smaller*D->k, smaller*D->k + D->k - 1);

    if( D->qvalues->vector[(index+1)*D->k - 1] < D->qvalues->vector[(smaller)*D->k])        
        return;

    min_heapify_kheap(D, smaller);
}

void get_otu_qmatrix_kheap(dmatrix *D, otu_t* out, int* out_size){
    otu_t otu_v;
    int new_size = D->qvalues->heap_size - D->k;
    *out_size = D->k;

    if (D->qvalues->heap_size <= D->k){
        *out_size = D->qvalues->heap_size;
        new_size = 0;
    }
        

    for (int i = 0; i < *out_size; i++)
    {
        otu_v.i = D->positions->vector[i] / D->N;
        otu_v.j = D->positions->vector[i] % D->N;
        D->qvalues->vector[0];
        out[i] = otu_v;

        D->positions->vector[i] = D->positions->vector[D->qvalues->heap_size - *out_size + i];
        D->qvalues->vector[i] = D->qvalues->vector[D->qvalues->heap_size - *out_size + i];
    }
    
    D->qvalues->heap_size = new_size;

    min_heapify_kheap(D, 0);
}

// otu_t get_otu_qmatrix(dmatrix *D){
//     otu_t otu_v;
//     otu_v.i = D->positions->vector[0] / D->N;
//     otu_v.j = D->positions->vector[0] % D->N;

//     int new_size = D->qvalues->heap_size-1;

//     // printf("%.2f\n", D->qvalues->vector[0]);
//     float qvalue = D->qvalues->vector[0];
//     D->qvalues->vector[0] = D->qvalues->vector[ new_size ];
//     D->positions->vector[0] = D->positions->vector[ new_size ];
//     D->qvalues->heap_size = new_size;

//     min_heapify(D, 0);

//     return otu_v;
// }

// Pode ser um vetor em D

// void calculate_q_matrix_min_n(dmatrix* D, otu_value_t *otus ){
//     const int n_minus_2 = D->N - 2;
//     float dij, q_value;
//     otu_value_t otu_min_line;
    
//     for (int i = 0; i < D->N; i++)
//     {
//         otu_min_line.q_value = 1e20;

//         for (int j = 0; j < i; j++)
//         {
//             dij = get_dmatrix(D, i, j);
//             q_value = n_minus_2*dij - get_sum(D, i) - get_sum(D, j);
            
//             if(q_value < otu_min_line.q_value){
//                 otu_min_line.i = i;
//                 otu_min_line.j = j;
//                 otu_min_line.q_value = q_value;
//             }

//         }

//         otus[i] = otu_min_line;

//     }
    
// }
