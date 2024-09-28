#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>

struct options{
    const char* filename;
    int heap;
    float p_value;
    int k;
};
typedef struct options options_t;

void die();
void dies(char* format, ...);

#endif
