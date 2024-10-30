#ifndef _HEAP_H
#define _HEAP_H

#include <stdlib.h>

struct heap_struct{
    float* elements;
    int size;
    int alloc;
};
typedef struct heap_struct heap_t;

heap_t* normal_heap(size_t alloc){
    heap_t* heap_r = (heap_t*) malloc(sizeof(heap_t));

    if(heap_r == NULL)
        return NULL;

    heap_r->elements = NULL;
    heap_r->alloc = 0;
    heap_r->size = 0;

    return heap_r;
}

void free_normal_heap(heap_t* nheap){
    free(nheap->elements);
    free(nheap);
}

void n_bottom_up_update(heap_t* heap, int node){
    int index, father, lesser;
    float temp;

    index = node;
    father = index/2;

    lesser = heap->elements[index] < heap->elements[father];

    while(lesser){
        temp = heap->elements[index];
        heap->elements[index] = heap->elements[father];
        heap->elements[father] = temp;

        if(father == 0) break;

        index = father;
        father = index/2;

        lesser = heap->elements[index] < heap->elements[father];
    }
}

void n_up_bottom_update(heap_t* heap, int node){
    int index, left, right;
    int lesser, less;
    float temp;

    index = node;
    right = index*2 + 1;
    left = index*2 + 2;
    
    // while(right < heap->size){

    //     if(left >= heap->size){
    //         if(heap->elements[right] < heap->elements[index]){
    //             temp = heap->elements[index];
    //             heap->elements[index] = heap->elements[right];
    //             heap->elements[right] = temp;
    //         }
    //         break;
    //     }

    //     lesser = left;

    //     if( heap->elements[right] < heap->elements[left])
    //         lesser = right;

    //     if(heap->elements[lesser] > heap->elements[index])
    //         break;
        
    //     temp = heap->elements[index];
    //     heap->elements[index] = heap->elements[lesser];
    //     heap->elements[lesser] = temp;

    //     index = lesser;
    //     right = index*2 + 1;
    //     left = index*2 + 2;
    // }

    if(right >= heap->size)
        return;
    
    if( left >= heap->size && heap->elements[right] < heap->elements[index]){
        temp = heap->elements[right];
        heap->elements[right] = heap->elements[index];
        heap->elements[index] = temp;
        return;
    }

    less = heap->elements[right] < heap->elements[index] || heap->elements[left] < heap->elements[index];

    while(less){
        
        if(right >= heap->size) break;

        if( left >= heap->size && heap->elements[right] < heap->elements[index]){
            temp = heap->elements[right];
            heap->elements[right] = heap->elements[index];
            heap->elements[index] = temp;
            break;
        }

        lesser = left;

        if(heap->elements[right] < heap->elements[left])
            lesser = right;

        temp = heap->elements[index];
        heap->elements[index] = heap->elements[lesser];
        heap->elements[lesser] = index;

        index = lesser;

        right = index*2 + 1;
        left = index*2 + 21;
    
        if(right >= heap->size) break;

        if( left >= heap->size && heap->elements[right] < heap->elements[index]){
            temp = heap->elements[right];
            heap->elements[right] = heap->elements[index];
            heap->elements[index] = temp;
            break;
        }

        less = heap->elements[right] < heap->elements[index] || heap->elements[left] < heap->elements[index];
    }
}

void insertion_heap(heap_t* heap, float item){
    if(heap->alloc <= heap->size + 1){
        heap->elements = (float*) realloc(heap->elements, (heap->alloc + 1)*sizeof(float));
        heap->alloc += 1;
    }

    heap->elements[heap->size] = item;
    heap->size += 1;

    if(heap->size == 1)
        return;

    n_bottom_up_update(heap, heap->size - 1);
}

void deletion_heap(heap_t* heap, float *item){
    *item = heap->elements[0];

    if(heap->size == 1){
        heap->size = 0;
        return;
    }

    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size -= 1;

    n_up_bottom_update(heap, 0);
}

#endif