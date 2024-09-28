#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "dmatrix.h"

dmatrix* dmatrix_alloc(options_t op, int N){
    int s_vector = N*(N-1)/2;

    dmatrix* D = malloc(sizeof(dmatrix));
    if(!D) return 0;
    
    D->N = N;
    D->p_value = op.p_value;
    D->k = op.k;

    D->positions = NULL;
    D->qvalues = NULL;

    D->distances = vector_alloc(s_vector);
    if( !(D->distances) ) return 0;

    D->sums = vector_alloc(N);
    if( !(D->sums) ) return 0;

    if(op.heap == 1){
        D->qvalues = vector_alloc(s_vector);
        if( !(D->qvalues) ) return 0;
        
        D->positions = vector_int_alloc(s_vector);
        if( !(D->positions) ) return 0;
    }
    
    return D;
}

void dmatrix_resize(dmatrix *D, int line_excluded){
    int i, n_og, n_new;
    float value, sum_last;

    n_og = D->N;
    n_new = D->N - 1;

    D->sums->size = n_new;
    D->N = n_new;
    
    if(line_excluded == n_new ) return; 

    sum_last = get_sum(D, n_new);
    set_sum(D, line_excluded, sum_last);

    for(i = 0; i < n_og; i++){
        if(i == line_excluded) continue;
        value = get_dmatrix(D, i, n_new);
        set_dmatrix(D, i, line_excluded, value);
    }
}

void dmatrix_free(dmatrix *D){
    if(!D) return;

    vector_free(D->distances);
    vector_free(D->sums);

    vector_free(D->qvalues);
    vector_int_free(D->positions);

    free(D);
}

dmatrix* dmatrix_load(options_t op){
    int N, i, j, err;
    float value, sum_i, sum_j;
    dmatrix* D = 0;

    FILE* f = fopen(op.filename, "r");
    if(!f) return 0;

    if (fscanf(f,"%d ",&N) != 1) goto READERRO;

    D = dmatrix_alloc(op, N);
    if(!D) goto ALLOCERRO;

    // Nomes

    
    // Distancias
    for(i = 0; i < N; ++i)
    {
        for (j = 0; j < i; ++j)
        {   
            if(fscanf(f, "%f;", &value) != 1) goto READERRO;
            set_dmatrix(D, i, j, value);

            sum_i = get_sum(D, i);
            sum_j = get_sum(D, j);
        
            set_sum(D, i, sum_i+value );
            set_sum(D, j, sum_j+value );
            
        }
        
    }

    fclose(f);

    return D;

    READERRO: err = EILSEQ; goto STDERRO;
    ALLOCERRO: err = errno; goto STDERRO;
    STDERRO:
        fclose(f);
        dmatrix_free(D);
        errno = err;
        return 0;
}

int dmatrix_pos(int i, int j){
    int aux_swap;

    if (i==j)
        return -1;

    if (j > i){
        aux_swap = i;
        i = j;
        j = aux_swap;
    }

    return i*(i-1)/2 + j;
}

float get_dmatrix(dmatrix *D, int i, int j){
    int pos_vector = dmatrix_pos(i, j);
    if(pos_vector < 0) return 0;

    return D->distances->vector[pos_vector];
}

void set_dmatrix(dmatrix* D, int i, int j, float value){
    int pos_vector =  dmatrix_pos(i, j);
    if(pos_vector < 0) return;
    
    D->distances->vector[pos_vector] = value;
}

float get_qmatrix(dmatrix *D, int i, int j){
    int pos_vector = dmatrix_pos(i, j);
    if(pos_vector < 0) return 0;

    return D->qvalues->vector[pos_vector];
}

void set_qmatrix(dmatrix* D, int i, int j, float value){
    int pos_vector =  dmatrix_pos(i, j);
    if(pos_vector < 0) return;
    
    D->qvalues->vector[pos_vector] = value;
}

float get_position_aux(dmatrix *D, int i, int j){
    int pos_vector = dmatrix_pos(i, j);
    if(pos_vector < 0) return 0;

    return D->positions->vector[pos_vector];
}

void set_position_aux(dmatrix* D, int i, int j, float value){
    int pos_vector =  dmatrix_pos(i, j);
    if(pos_vector < 0) return;
    
    D->positions->vector[pos_vector] = value;
}

float get_sum(dmatrix* D, int pos){
    return D->sums->vector[pos];
}

void set_sum(dmatrix* D, int pos, float value){
    D->sums->vector[pos] = value;
}

void dmatrix_print(dmatrix* D){
    printf("D: \n");
    for(int i = 1; i < D->N; ++i){
        for(int j = 0; j < i; ++j){
            printf("%05.2f\t", get_dmatrix(D, i, j));
        }
        printf("\n");
    }


    printf("S: \n");
    int sums_size = D->sums->size;
    for(int i = 0; i < sums_size; ++i){
        printf("%05.2f\t",  get_sum(D, i) );
    }
    printf("\n\n");
}