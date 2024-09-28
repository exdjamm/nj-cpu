#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define BLOCK_SIZE 16

typedef struct{
    int size;
    float *elements;
} Vector;

typedef struct{
    int n;
    float *elements;
} Matrix;


// Matrix
__device__ float d_get_matrix_position(Matrix D, int i, int j);
// float h_get_position(Matrix D, int i, int j);

__device__ void d_set_matrix_position(Matrix D, int i, int j, float value);
void h_set_matrix_position(Matrix D, int i, int j, float value);

__global__ void min_Q(Matrix D, Vector S, Matrix Q);

int main(int argc, char const *argv[])
{
    int N, iter ;
    float value, sum_i, sum_j;

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

    Matrix h_D, d_D, d_Q;
    Vector h_S, d_S;
    size_t size_D, size_S;

    if(argc != 2){
      printf("missing filename\n");
      return 0;
    }

    FILE* f = fopen(argv[1], "r");
    if(!f) return 0;

    if (fscanf(f,"%d ",&N) != 1) goto READERRO;

    size_D = (N*(N-1)/2)*sizeof(float);
    h_D.elements = (float*) malloc(size_D);
    if(h_D.elements == NULL) goto EXIT;

    size_S = N*sizeof(float);
    h_S.elements = (float*) calloc(N, sizeof(float));
    if(h_S.elements == NULL) goto EXIT;

    // Distancias
    for(int i = 0; i < N; ++i)
    {
        for (int j = 0; j < i; ++j)
        {   
            if(fscanf(f, "%f;", &value) != 1) goto READERRO;
            h_set_matrix_position(h_D, i, j, value);   
            
            sum_i = h_S.elements[i];
            sum_j = h_S.elements[j];

            h_S.elements[i] += sum_i + value;
            h_S.elements[j] += sum_j + value;
            // printf("%f, %d-%d\n", value, i, j);
        }
        
    }

    fclose(f);

    cudaMalloc(&d_D.elements, size_D);
    cudaMalloc(&d_S.elements, size_S);
    cudaMemcpy(d_D.elements, h_D.elements, size_D, cudaMemcpyHostToDevice);
    cudaMemcpy(d_S.elements, h_S.elements, size_S, cudaMemcpyHostToDevice);

    cudaMalloc(&d_Q.elements, size_D);

    h_D.n = N;
    h_S.size = N;
    d_D.n = N;
    d_S.size = N;
    d_Q.n = N;

    // A EXECUCAO DO NJ É "LINEAR" (dependente de uma situcao anteior)
    // Somente e possivel calcular as novas distancias depois de selecionar o par
    iter = N;

    while(iter == N){

        // se iter for menor que block_size, ele nao executa nada pq vai ser zero;
        dim3 dimGrid(iter/dimBlock.x, iter/dimBlock.y);
        min_Q<<<dimGrid, dimBlock>>>(d_D, d_S, d_Q);
        cudaDeviceSynchronize();
        // q_heap<<<aaaa>>>()
        // update_D<<<aaaa>>>()

        iter--;
    }

    cudaFree(d_D.elements);
    cudaFree(d_S.elements);
    cudaFree(d_Q.elements);

    free(h_D.elements);
    free(h_S.elements);

    return 0;

    READERRO:
    printf("Erro de leitura");
    goto EXIT;

    EXIT:
    fclose(f);
    free(h_D.elements);
    free(h_S.elements);
    cudaFree(d_D.elements);
    cudaFree(d_S.elements);
    cudaFree(d_Q.elements);
    return 1;
    
    return 0;
}

void h_set_matrix_position(Matrix D, int i, int j, float value){
    int pos;

    if(i==j)
        return;

    pos = i*(i-1)/2 + j;
    if(i < j)
        pos = j*(j-1)/2 + i;
    D.elements[pos] = value;
}

__device__ void d_set_matrix_position(Matrix D, int i, int j, float value){
    int pos;

    if(i==j)
        return;

    pos = i*(i-1)/2 + j;
    if(i < j)
        pos = j*(j-1)/2 + i;
    D.elements[pos] = value;
}

__device__ float d_get_matrix_position(Matrix D, int i, int j){
    int pos;

    if(i==j)
        return 0;

    pos = i*(i-1)/2 + j;
    if(i < j)
        pos = j*(j-1)/2 + i;
    return D.elements[pos] ;
}

// float h_get_position(Matrix D, int i, int j){

// }

__global__ void min_Q(Matrix D, Vector S, Matrix Q){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    float value, d_rc;

    if((row>=D.n) && (col>=D.n)) return;
    
    d_rc = d_get_matrix_position(D, row, col);
    value = (D.n-2)*d_rc - S.elements[row] - S.elements[col];

    printf("%d - %d\n", row, col);
    d_set_matrix_position(Q, row, col, value);
}