#ifndef _UTILS_H
#define _UTILS_H

#include <time.h>

void set_time(float *starting_time){
    *starting_time = clock();
}

float get_time(float starting_time, float ending_time){
    return 1000.0 * ( (float) (ending_time - starting_time) )/CLOCKS_PER_SEC;
}

#endif