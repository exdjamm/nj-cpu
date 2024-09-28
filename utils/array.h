#ifndef ARRAY_H_
#define ARRAY_H_

struct string{
    int size;
    char value[256];
};
typedef struct string string_t;

typedef struct{
    int size;
    int heap_size;
    float *vector;
} vector_f;

typedef struct{
    int size;
    int *vector;
} vector_i;

vector_f* vector_alloc(int size);
void vector_free(vector_f* v);

vector_i* vector_int_alloc(int size);
void vector_int_free(vector_i* v);


#endif
