#include <stdlib.h>
#include "array.h"

vector_f* vector_alloc(int size){
    vector_f* v = malloc(sizeof(vector_f));

    if(!v) return 0;
    
    v->size = size;
    v->heap_size = size;
    v->vector = calloc(size, sizeof(float));
    if(!v->vector) return 0;
    
    return v;
}

void vector_free(vector_f* v){
    if(!v) return;
    
    free(v->vector);
    free(v);
}

vector_i* vector_int_alloc(int size){
    vector_i* v = (vector_i*) malloc(sizeof(vector_i));

    if(!v) return 0;
    
    v->size = size;
    v->vector = (int*) calloc(size, sizeof(int));
    if(!v->vector) return 0;
    
    return v;
}

void vector_int_free(vector_i* v){
    if(!v) return;
    
    free(v->vector);
    free(v);
}
