#include "comum.h"

float distance_uk_heap(dmatrix* D, int i, int j, int k){
    float d_ij  = get_dmatrix(D, i, j);
    float d_ik = get_dmatrix(D, i, k);
    float d_jk = get_dmatrix(D, j, k);

    return (d_ik + d_jk - d_ij)/2;
}

void calculate_q_matrix(dmatrix* D){
    const int n_minus_2 = D->N - 2;
    float dij, q_value;

    for (int i = 0; i < D->N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            dij = get_dmatrix(D, i, j);
            q_value = n_minus_2*dij - get_sum(D, i) - get_sum(D, j);
            set_qmatrix(D, i, j, q_value);
            set_position_aux(D, i, j, i*D->N + j);
        }
    }

    D->qvalues->heap_size = D->N*(D->N-1)/2;
    
}

