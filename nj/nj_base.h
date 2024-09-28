#ifndef NJ_BASE_H_
#define NJ_BASE_H_

#include "../utils/dmatrix.h"

void nj_base(dmatrix *D);

step_info next_otu(dmatrix *D);

float distance_uk(dmatrix* D, int i, int j, int k);
void dmatrix_update(dmatrix *D, step_info data);

#endif