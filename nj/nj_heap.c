#include <stdio.h>
#include <stdlib.h>

#include "nj_heap.h"

void calculate_q_matrix(dmatrix* D);
void calculate_q_matrix_min_n(dmatrix* D, otu_value_t *otus);

void min_heapify(dmatrix* D, int index);
void heapify_qmatrix(dmatrix* D);

otu_t get_otu_qmatrix(dmatrix *D);

void min_heapify_otu_value(otu_value_t* otus, int size, int index);

void dmatrix_update_heap(dmatrix *D, otu_t data);
float distance_uk_heap(dmatrix* D, int i, int j, int k);

void nj_heap(dmatrix *D){
    int accumulate, collect;
    int *collected =  calloc(sizeof(int), D->N);

    collect = D->N*D->p_value;
    if(collect == 0) collect = 1;

    otu_t otus[collect];

    while(D->N >= 3){

        for (int i = 0; i < D->N; i++){
            collected[i] = 0;
        }
        
        calculate_q_matrix(D);
        heapify_qmatrix(D);

        accumulate = 0;

        while(accumulate < collect){
            otu_t otu_v = get_otu_qmatrix(D);

            
            if( collected[otu_v.i] == 0 && collected[otu_v.j] == 0){
                collected[otu_v.i] = 1;
                collected[otu_v.j] = 1;
                otus[accumulate] = otu_v;

                accumulate++;
            }
        }
    
        for (int i = 0; i < collect; i++){
            dmatrix_update_heap(D, otus[i]);
            
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
}

void dmatrix_update_heap(dmatrix *D, otu_t data){
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

void heapify_qmatrix(dmatrix* D){
    int size = D->qvalues->heap_size;

    for (int i = size/2; i >= 0; i--)
    {
        min_heapify(D, i);
    }
}

void min_heapify(dmatrix* D, int index){
    int l_child, r_child, lesser;
    l_child = 2*index + 1;
    r_child = 2*index + 2;

    float temp;
    int temp_int;

    if( l_child < D->qvalues->heap_size && D->qvalues->vector[l_child] < D->qvalues->vector[index] )
        lesser = l_child;
    else
        lesser = index;
    
    if( r_child < D->qvalues->heap_size && D->qvalues->vector[r_child] < D->qvalues->vector[lesser] )
        lesser = r_child;

    if(lesser == index) return;

    temp = D->qvalues->vector[index]; 
    D->qvalues->vector[index] = D->qvalues->vector[lesser];
    D->qvalues->vector[lesser] = temp;

    temp_int = D->positions->vector[index]; 
    D->positions->vector[index] = D->positions->vector[lesser];
    D->positions->vector[lesser] = temp_int;

    min_heapify(D, lesser);
}

otu_t get_otu_qmatrix(dmatrix *D){
    otu_t otu_v;
    otu_v.i = D->positions->vector[0] / D->N;
    otu_v.j = D->positions->vector[0] % D->N;

    int new_size = D->qvalues->heap_size-1;

    // printf("%.2f\n", D->qvalues->vector[0]);
    float qvalue = D->qvalues->vector[0];
    D->qvalues->vector[0] = D->qvalues->vector[ new_size ];
    D->positions->vector[0] = D->positions->vector[ new_size ];
    D->qvalues->heap_size = new_size;

    min_heapify(D, 0);

    return otu_v;
}

// Pode ser um vetor em D

void calculate_q_matrix_min_n(dmatrix* D, otu_value_t *otus ){
    const int n_minus_2 = D->N - 2;
    float dij, q_value;
    otu_value_t otu_min_line;
    
    for (int i = 0; i < D->N; i++)
    {
        otu_min_line.q_value = 1e20;

        for (int j = 0; j < i; j++)
        {
            dij = get_dmatrix(D, i, j);
            q_value = n_minus_2*dij - get_sum(D, i) - get_sum(D, j);
            
            if(q_value < otu_min_line.q_value){
                otu_min_line.i = i;
                otu_min_line.j = j;
                otu_min_line.q_value = q_value;
            }

        }

        otus[i] = otu_min_line;

    }
    
}

void heapify_otu_value(otu_value_t * otus, int size){

    for (int i = size/2; i >= 0; i--)
    {
        min_heapify_otu_value(otus, size, i);
    }
}

void min_heapify_otu_value(otu_value_t* otus, int size, int index){
    int l_child, r_child, lesser;
    l_child = 2*index + 1;
    r_child = 2*index + 2;

    otu_value_t temp;

    if( l_child < size && otus[l_child].q_value < otus[index].q_value )
        lesser = l_child;
    else
        lesser = index;
    
    if( r_child < size && otus[r_child].q_value < otus[lesser].q_value )
        lesser = r_child;

    if(lesser == index) return;

    temp = otus[index]; 
    otus[index] = otus[lesser];
    otus[lesser] = temp;

    min_heapify_otu_value(otus, size, lesser);
}

otu_t get_otu_otus(otu_value_t *otus, int size){
    otu_t otu_v;
    otu_v.i = otus[0].i;
    otu_v.j = otus[0].j;

    int new_size = size-1;

    otus[0] = otus[ new_size ];
    size = new_size;

    min_heapify_otu_value(otus, size, 0);

    return otu_v;
}