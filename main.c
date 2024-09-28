#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "nj.h"
#include "./utils/utils.h"

int main(int argc, char const *argv[])
{   
    options_t op;
    float dtime, time_cpu;

    op.heap = 0;
    op.p_value = 0;

    if(argc == 3){
        op.heap = 1;
        op.p_value = strtof(argv[2], NULL);
        op.k = 0;
    }else if(argc == 4){
        op.heap = 1;
        op.p_value = strtof(argv[2], NULL);
        op.k = strtol(argv[3], NULL, 10);
    }
    else if( argc <= 1 || argc > 4)
        dies("Numero de argumentos insuficiente.\n");
    
    op.filename = argv[1];
    
    // if(!op.heap)
    //     printf("[NJ] DEFAULT\n");
    // else{
    //     printf("[NJ] FLEX %.3f\n", op.p_value);
    // }
    // printf("[NJ] FILE: %s\n", op.filename);

    nj(op, &dtime);
    time_cpu =  1000*( (float) dtime) / CLOCKS_PER_SEC;
    printf("%.3f;\n", time_cpu);

    return 0;
}