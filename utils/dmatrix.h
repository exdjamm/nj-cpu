#ifndef DMATRIX_H_
#define DMATRIX_H_

#include "array.h"
#include "utils.h"

typedef struct {
    vector_f* distances;
    vector_f* sums;

    vector_f* qvalues;
    vector_i* positions;
    // string_t* names;
    int N;
    float p_value;
    int k;
} dmatrix;

struct otu{
    int i, j;
    char* name;
};
typedef struct otu otu_t;

struct otu_value{
    int i, j;
    float q_value;
    char* name;
};
typedef struct otu_value otu_value_t;

typedef struct {
    int i, j;
    float d_i, d_j;

    float distances_ij;
} step_info;

void dmatrix_print(dmatrix* D);
dmatrix* dmatrix_load(options_t op);

dmatrix* dmatrix_alloc(options_t op, int N);
void dmatrix_resize(dmatrix* D, int pos);
void dmatrix_free(dmatrix* D);

float get_dmatrix(dmatrix* D, int i, int j);
void set_dmatrix(dmatrix* D, int i, int j, float value);

float get_qmatrix(dmatrix* D, int i, int j);
void set_qmatrix(dmatrix* D, int i, int j, float value);

float get_position_aux(dmatrix* D, int i, int j);
void set_position_aux(dmatrix* D, int i, int j, float value);

float get_sum(dmatrix* D, int pos);
void set_sum(dmatrix* D, int pos, float value);

#endif
