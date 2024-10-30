#ifndef _NJ_KHEAP_H
#define _NJ_KHEAP_H

#include <stdlib.h>

struct kheap_struct{
    float* heap;
    size_t k;
    size_t size;
    size_t alloc;
};

typedef struct kheap_struct kheap_t;

kheap_t* heap(int k, size_t init_memlloc_size);
void free_heap(kheap_t* kheap);

void insertion(kheap_t* kheap, float* items);
void insertion_unit(kheap_t* kheap, float item);

void deletion(kheap_t* kheap, float* items);
void deletion_unit(kheap_t* kheap, float item);

#endif