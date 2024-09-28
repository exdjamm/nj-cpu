#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <time.h>

#include <errno.h>

#include "nj.h"

void nj(options_t op, float *dtime){
    dmatrix* D = dmatrix_load(op);

    if(!D)
        return;
    
    *dtime = clock();
    if(op.heap){

        if(op.k == 0)
            nj_heap(D);
        else
            nj_kheap(D);
    }else{
        nj_base(D);
    }
    *dtime = clock() - *dtime;

    dmatrix_free(D);
}

