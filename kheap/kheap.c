#include <stdio.h>
#include <stdlib.h>

#include "kheap.h"

int verify_intersection_or_lesser(kheap_t* kheap, int first, int second);
int smaller_btwn(kheap_t* kheap, int first, int second);
void merge(kheap_t* kheap, int nodep, int nodeq);

void up_bottom_update(kheap_t* kheap, int node);
void bottom_up_update(kheap_t* kheap, int node);

void print_node(kheap_t* kheap, int node){
    for (int i = 0; i < kheap->k; i++)
    {
        printf("%.3f ", kheap->heap[node*kheap->k + i]);
    }
    printf("\n");
}

int fltcompar(const void* p, const void* q){
    float x, y;
    x = *(const float*)p;
    y = *(const float*)q;

    return (x > y) - (x < y);
}

int father_node(int index){
    return index/2;
}
int right_node(int index){
    return index*2 + 1;
}
int left_node(int index){
    return index*2 + 2;
}

kheap_t* heap(int k, size_t init_memlloc_size){
    kheap_t* kheap = (kheap_t*) malloc(sizeof(kheap_t));

    if(kheap == NULL)
        return NULL;

    kheap->heap = NULL;
    kheap->k = k;
    kheap->size = 0;
    kheap->alloc = 0;

    return kheap;
}

void free_heap(kheap_t* kheap){
    free(kheap->heap);
    free(kheap);
}

void insertion(kheap_t *kheap, float* items){
    int i;
    qsort(items, kheap->k, sizeof(float), fltcompar);

    if( kheap->alloc <= kheap->size + kheap->k){
        kheap->heap = (float*) realloc(kheap->heap, (kheap->alloc + kheap->k)*sizeof(float));
        kheap->alloc += kheap->k;
    }
        
    for(i = 0; i < kheap->k; i++)
        kheap->heap[kheap->size + i] = items[i];
    kheap->size += kheap->k;

    if(kheap->size == kheap->k)
        return;

    bottom_up_update(kheap, (kheap->size/kheap->k)-1);
}

void deletion(kheap_t* kheap, float* items){
    for (int i = 0; i < kheap->k && i < kheap->size; i++){
        items[i] = kheap->heap[i];
        if(kheap->size/kheap->k - 1 >= 1){
            kheap->heap[i] = kheap->heap[kheap->size - kheap->k + i];
        }
    }

    if(kheap->size < kheap->k){
        kheap->size = 0;
        return;
    }
    
    kheap->size -= kheap->k;

    up_bottom_update(kheap, 0);
}

void build_kheap(kheap_t* kheap){

    for(int i = kheap->size/(2*kheap->k); i >=0; --i){
        up_bottom_update(kheap, i);
    }
}

void bottom_up_update(kheap_t* kheap, int node){
    int index, father, btw;

    index = node;
    father = father_node(index);

    btw = verify_intersection_or_lesser(kheap, father, index);

    while(btw){

        merge(kheap, father, index);
        if( father == 0 ) break;

        index = father;
        father = father_node(index);

        btw = verify_intersection_or_lesser(kheap, father, index);
    }
}

void up_bottom_update(kheap_t* kheap, int node){
    int index, left, right, chg_smaller;
    int btw, smll_btw;
    float temp;

    index = node;
    right = right_node(index);
    left = left_node(index);
    
    chg_smaller = 0;

    btw = verify_intersection_or_lesser(kheap, index, right) || verify_intersection_or_lesser(kheap, index, left);

    // Codigo muito eficiente? 
    while(btw){
        if(right >= kheap->size/kheap->k) break;

        if( left >= kheap->size/kheap->k){
            merge(kheap, index, right);
            break;
        }

        smll_btw = smaller_btwn(kheap, right, left);
        if (smll_btw > 0)
            chg_smaller = 1;
        
        merge(kheap, right, left);
        merge(kheap, index, right); // poderia ser apenas trocar O(K)

        index = right;

        if(chg_smaller){
            index = left;

            for (int i = 0; i < kheap->k; i++){
                temp = kheap->heap[right*kheap->k + i];
                kheap->heap[right*kheap->k + i] = kheap->heap[left*kheap->k + i];
                kheap->heap[left*kheap->k + i] = temp;
            }
        }

        right = right_node(index);
        left = left_node(index);

        btw = verify_intersection_or_lesser(kheap, index, right) || verify_intersection_or_lesser(kheap, index, left);
    }

    // int lesser, down;

    // while(right < kheap->size/kheap->k){
    //     if(left >= kheap->size/kheap->k){
    //         merge(kheap, index, right);
    //         break;
    //     }
    //     down = right;

    //     lesser = smaller_btwn(kheap, right, left);
    //     if (lesser == 1)
    //         down = left;

    //     btw = verify_intersection_or_lesser(kheap, index, right) || verify_intersection_or_lesser(kheap, index, left);

    //     if(!btw){
    //         break;
    //     }
        
    //     merge(kheap, right, left);
    //     merge(kheap, index, right);

    //     if (lesser == 1)
    //     {
    //         for (int i = 0; i < kheap->k; i++){
    //             temp = kheap->heap[right*kheap->k + i];
    //             kheap->heap[right*kheap->k + i] = kheap->heap[left*kheap->k + i];
    //             kheap->heap[left*kheap->k + i] = temp;
    //         }
    //     }
        
    //     index = down;
    //     right = right_node(index);
    //     left = left_node(index);

    // }
}

int verify_intersection_or_lesser(kheap_t* kheap, int first, int second){
    int node_size = kheap->size/kheap->k;
    float minp, maxp, minq, maxq;

    int lesser, intersection;
    lesser = intersection = 0;

    if (first >= node_size || second >= node_size)
        return 0;
    
    minp = kheap->heap[first*kheap->k];
    maxp = kheap->heap[(first+1)*kheap->k - 1];

    minq = kheap->heap[second*kheap->k];
    maxq = kheap->heap[(second+1)*kheap->k - 1];

    if( minq < minp)
        lesser = 1;
    if( minq < maxp || maxq < minp || maxq < maxp)
        intersection = 1;

    return lesser || intersection;
}

int smaller_btwn(kheap_t* kheap, int first, int second){
    int node_size = kheap->size/kheap->k;
    float maxp, maxq;

    if (first >= node_size || second >= node_size)
        return 0;

    maxp = kheap->heap[(first+1)*kheap->k - 1];
    maxq = kheap->heap[(second+1)*kheap->k - 1];

    return (maxp > maxq) - (maxp < maxq);
}

void merge(kheap_t* kheap, int nodep, int nodeq){
    float *temp = (float*) calloc(sizeof(float), 2*(kheap->k));
    int i, j, k;
    i = j = k = 0;

    while(i < kheap->k && j < kheap->k){
        if(kheap->heap[nodep*kheap->k + i] < kheap->heap[nodeq*kheap->k + j]){
            temp[k] = kheap->heap[nodep*kheap->k + i];
            i++;
        }else{
            temp[k] = kheap->heap[nodeq*kheap->k + j];
            j++;
        }
        k++;
    }   

    while( i < kheap->k){
        temp[k] = kheap->heap[nodep*kheap->k + i];
        i++;
        k++;
    }

    while( j < kheap->k){
        temp[k] = kheap->heap[nodeq*kheap->k + j];
        j++;
        k++;
    }
    
    i = j = k = 0;
    while( i < kheap->k){
        kheap->heap[nodep*kheap->k + i] = temp[k];
        i++;
        k++;
    }
    while( j < kheap->k){
        kheap->heap[nodeq*kheap->k + j] = temp[k];
        k++;
        j++;
    }

    free(temp);
}

// void merge(float* org, int start, int middle, int end, float* temp){
//     int i, j, k;
//     i = start;
//     j = middle + 1;
//     k = 0;

//     while (i <= middle && j <= end){
//         if (org[i] < org[j]){
//             temp[k] = org[i];
//             i++;
//         }
//         else{
//             temp[k] = org[j];
//             j++;
//         }

//         k++;
//     }

//     while ( i <= middle ){
//         temp[k] = org[i];
//         k++;
//         i++;
//     }

//     while ( j <= end ){
//         temp[k] = org[j];
//         k++;
//         j++;
//     }

//     for (i = start; i < end+1; i++)
//     {
//         org[i] = temp[i-start];
//     }
// }