#include <stdio.h>

#include "nj_base.h"

void nj_base(dmatrix *D){
    while (D->N >= 3){

        step_info data = next_otu(D);
        dmatrix_update(D, data);
    }
}

step_info next_otu(dmatrix* D){
    int i, j;
    int n_minus_2 = D->N-2;
    float actual_min_q = 0, min_q, distance_ij, dif_sums;
    float product = (2*n_minus_2);
    
    step_info r;

    for(i = 1; i < D->N; ++i ){
        for(j = 0; j < i; ++j){
            distance_ij = get_dmatrix(D, i, j);
            min_q = n_minus_2*distance_ij - get_sum(D, i) - get_sum(D, j);

            if(min_q < actual_min_q){
                actual_min_q = min_q;
                r.i = i;
                r.j = j;
            }
        }
    }

    // printf("%.3f\n", actual_min_q);

    r.distances_ij = get_dmatrix(D, r.i, r.j);
    dif_sums = get_sum(D, r.i) - get_sum(D, r.j);

    r.d_i =  r.distances_ij/2 + dif_sums/product;
    r.d_j = r.distances_ij - r.d_i;

    return r;
}

float distance_uk(dmatrix* D, int i, int j, int k){
    float d_ij  = get_dmatrix(D, i, j);
    float d_ik = get_dmatrix(D, i, k);
    float d_jk = get_dmatrix(D, j, k);

    return (d_ik + d_jk - d_ij)/2;
}

void dmatrix_update(dmatrix *D, step_info data){
    int k;
    // abaixo, gambiarra, intuicao sem prova nem definicao
    float sum_u = get_sum(D, data.j);
    float sub = (D->N - 2)*data.d_j + data.distances_ij;
    set_sum(D, data.j, sum_u-sub); 

    for(k = 0; k < D->N; ++k){
        if( (k == data.i) || (k == data.j))
            continue;
        
        float d_uk = distance_uk(D, data.i, data.j, k);
        
        float d_ik = get_dmatrix(D, data.i, k);
        float d_jk = get_dmatrix(D, data.j, k);
        
        float sum_k = get_sum(D, k);
        sum_k += d_uk - d_ik - d_jk;
        set_sum(D, k, sum_k);

        set_dmatrix(D, data.j, k, d_uk);   
    }

    dmatrix_resize(D, data.i);
}