#include <stdlib.h>
#include <stdio.h>

#include "kheap.h"
#include "heap.h"
#include "utils.h"

int main(int argc, char const *argv[]){

    if(argc != 4){
        fprintf(stderr, "[ k number ] [ elements number ] [test type]\n");
        return -1;
    }

    int k = atoi(argv[1]);
    int elements_number = atoi(argv[2]);
    int test_type = atoi(argv[3]);

    int nodes_number = elements_number/k;
    float starting_time, ending_time, insertion_time, deletion_time;

    kheap_t* kheap = heap(k, 256);
    if (kheap == NULL)
        return -1;

    if(test_type == 0){ // insertion & deletion with batchs
        float *items = (float*) malloc(sizeof(float)*k*nodes_number);

        for (int i = 0; i < nodes_number*k; i++)
            items[i] = ((float) rand())/RAND_MAX;

        set_time(&starting_time);
        for (int i = 0; i < nodes_number; ++i)
            insertion(kheap, items+i*k);
        set_time(&ending_time);

        insertion_time = get_time(starting_time, ending_time);

        set_time(&starting_time);
        for (int i = 0; i < nodes_number; ++i)
            deletion(kheap, items+i*k);
        set_time(&ending_time);

        deletion_time = get_time(starting_time, ending_time);

        printf("insertion&deletion;%d;%d;%d;%.3f;%.3f;\n", k, nodes_number, k*nodes_number, insertion_time, deletion_time);
        
        free(items);
    }
    // else if(test_type == 1){ // unit insertion & deletion in batchs
    //     float *items = (float*) malloc(sizeof(float)*elements_number);

    //     for (int i = 0; i < elements_number; i++)
    //         items[i] = ((float) rand())/RAND_MAX;

    //     set_time(&starting_time);
    //     for (int i = 0; i < elements_number; ++i)
    //         insertion_unit(kheap, items[i]);
    //     set_time(&ending_time);

    //     insertion_time = get_time(starting_time, ending_time);
    //     printf("insertion_unit&deletion,%d,%d,%d,%.3f,%.3f\n", k, nodes_number, elements_number, insertion_time, deletion_time);
    // }
    else if(test_type == 2) // normal heap
    {
        heap_t* nheap = normal_heap(0);
        
        float *items = (float*) malloc(sizeof(float)*elements_number);

        for (int i = 0; i < elements_number; i++)
            items[i] = ((float) rand())/RAND_MAX;

        set_time(&starting_time);
        for (int i = 0; i < elements_number; ++i)
            insertion_heap(nheap, items[i]);
        set_time(&ending_time);

        insertion_time = get_time(starting_time, ending_time);

        set_time(&starting_time);
        for (int i = 0; i < elements_number; ++i)
            deletion_heap(nheap, items);
        set_time(&ending_time);

        deletion_time = get_time(starting_time, ending_time);

        printf("insertion&deletion_nheap;%d;%d;%d;%.3f;%.3f;\n", k, nodes_number, k*nodes_number, insertion_time, deletion_time);
        
        free(items);

        free_normal_heap(nheap);
    }
    

    free_heap(kheap);

    return 0;
}
