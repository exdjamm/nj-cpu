#ifndef NJ_COMUM_H_
#define NJ_COMUM_H_

#include "../utils/dmatrix.h"

void calculate_q_matrix(dmatrix* D);
float distance_uk_heap(dmatrix* D, int i, int j, int k);

#endif