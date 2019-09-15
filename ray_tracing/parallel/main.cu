#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <cuda.h>
#undef __STRICT_ANSI__
#include <math.h>
//#include <time.h> 
//#include <omp.h>
//#include "my_vector.h"
#include "atomic_add.h"

#define W_MAX 10

double ** allocate_G(int n) {
	double ** rv = (double **)calloc(n, sizeof(double *));
	for(int i = 0; i < n; i++) {
		rv[i] = (double *)calloc(n, sizeof(double));
	}
	
	return rv;
}

__device__ double LCG_random_double(uint64_t * seed) {
	const uint64_t m = 9223372036854775808ULL;
	const uint64_t a = 2806196910506780709ULL;
	const uint64_t c = 1ULL;
	*seed = (a* (*seed) + c) % m;
	return (double) (*seed)/ (double)m;
}

__device__ int translation(double a, double start, double end, double N, int axis) {
	double cell_size = (end - start)/N;
	if( cell_size < 0 ) 
		cell_size *= -1;
	int rv = 0;
	assert(axis == 0 || axis == 1);
	if( axis == 0 ) {
		while(!(start <= a && a < start + cell_size)) { 
			rv++;
			start += cell_size;
			assert(start <= end);
		}
	} else {
		while(!(start >= a && a > start - cell_size)) {
			rv++;
			start -= cell_size;
			assert(start >= end);
		} 
	}
	return rv;
}

__device__ double inner(double A [], double B [], int length);
__device__ static inline int condition_fast(double W_x, double W_z, double V [], double C [], double R) {
	//printf("this is W_x %lf, W_z %lf \n", W_x, W_z);
	if( (-1 * W_MAX < W_x) && (W_x < W_MAX) && (-1 * W_MAX < W_z) && (W_z < W_MAX)) {
		double inner_product = inner(V, C, 3);
		double norm = inner(C, C, 3);
		//printf("this is condition %lf\n", inner * inner + R * R - norm);
		if( inner_product * inner_product + R * R - norm > 0) {
			return 1; 
		}
		return 0;
	}
	return 0;
}


__device__ double inner(double A [], double B [], int length) {
	double rv = 0;
	for(int i = 0; i < length; i++) {
		rv += A[i] * B[i];
	}
	return rv;
}

__global__ void one_iteration_fast(int n, double * G_D) {
	double W_y = 10;
	double R = 6;

	double L [3]; 
	double C [3];
	L[0] = 4; L[1] = 4; L[2] = -1;
	C[0] = 0; C[1] = 12; C[2] = 0;

	double phi = -1;
	double cos_theta = -2;
	double sin_theta = -2;

	double V [3];
	double W [3];
	double I [3];
	double N [3];
	double S [3];

	double W_x = -1;
	double W_z = -1;
	double t = 0;
	double b = -1;
	int i = 0;
	int j = 0;
	uint64_t seed = (blockDim.x * blockIdx.x + threadIdx.x) * 4238811;
	//assert(0 == 1);

	do { 
		phi = 2 * M_PI * LCG_random_double(&seed); // this should be 0 to 2 pi 			
		cos_theta = 2 * LCG_random_double(&seed) - 1; // this should be -1 to 1
		sin_theta = sqrt( 1 - cos_theta * cos_theta);
		assert(0 < phi && phi < 2 * M_PI); assert(-1 < cos_theta && cos_theta < 1); assert(-1 <= sin_theta && sin_theta <= 1);
		V[0] = sin_theta * cos(phi); V[1] = sin_theta * sin(phi); V[2] = cos_theta; 
		W[0] = V[0] * W_y/V[1]; W[1] = V[1] * W_y/V[1]; W[2] = V[2] * W_y/V[1];
		W_x = W[0];
		W[1] = W_y;
		W_z = W[2];
	} while (condition_fast(W_x, W_z, V, C, R) == 0);
	double a = inner(V, C, 3);
	assert(inner(C, C, 3) >= 0);
	t = a - sqrt(a * a + R * R - inner(C, C, 3));
	I[0] = t * V[0]; I[1] = t * V[1]; I[2] = t * V[2];

	// calculating N here 
	N[0] = I[0] - C[0]; N[1] = I[1] - C[1]; N[2] = I[2] - C[2];
	a = sqrt(inner(N, N, 3));
	assert(a != 0);
	N[0] = 1/a * N[0]; N[1] = 1/a * N[1]; N[2] = 1/a * N[2];

	// calculating S here
	S[0] = L[0] - I[0]; S[1] = L[1] - I[1]; S[2] = L[2] - I[2];
	a = sqrt(inner(S, S, 3));
	assert(a != 0);
	S[0] = 1/a * S[0]; S[1] = 1/a * S[1]; S[2] = 1/a * S[2];

	// change b accordingly
	b = inner(S, N, 3);
	if(b < 0) b = 0;

	i = translation(W_x, W_MAX, -1 * W_MAX, n, 1);
	j = translation(W_z, -1 * W_MAX, W_MAX, n, 0);
	assert(0 <= i); assert(i < n); assert(0 <= j); assert(j < n);
	atomicAdd_(&(G_D[i * n + j]), b);
}

int main(int argc, char ** argv) {
	assert(argc == 4);
	int n = atoi(argv[1]);
	int num_block = atoi(argv[2]);
	int threads_per_block = atoi(argv[3]);
	

	double * G = (double *)malloc(n*n *sizeof(double));
	for(int i = 0; i < n*n; i++) {
		G[i] = 0;
	}
	//G = allocate_G(n);
	double * G_D;
	assert(cudaMalloc((void **)&G_D, sizeof(double) * n * n) == cudaSuccess);
	//assert(G[n * n - 1] == 0);

	for(int i = 0; i < n * n; i++ ) {
		assert(cudaMemcpy(&(G_D[i]), &(G[i]), sizeof(double), cudaMemcpyHostToDevice) == cudaSuccess);
	}

	//printf("going through the iterations with %d blocks with %d threads\n", num_block, threads_per_block);
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start);
	one_iteration_fast<<<num_block, threads_per_block>>>(n, G_D);
	cudaError_t err;
	if((err = cudaGetLastError()) != cudaSuccess) { 
		const char * str = cudaGetErrorName(err);
		printf("%s\n", str);
	}
	cudaEventRecord(stop);

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			err = cudaMemcpy(G + i * n + j,G_D + i * n + j, sizeof(double),cudaMemcpyDeviceToHost);
			if(err != cudaSuccess){
				const char * str = cudaGetErrorName(err);
				printf("%s\n", str);
			}
		}
	}
	
	cudaEventSynchronize(stop);

	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("it took %f microseconds when %d blocks and %d threads are used\n", milliseconds, num_block, threads_per_block);

	// print to out.csv
	
	FILE * fp = fopen("out.csv", "w");
	//FILE * fp = stdout;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			if( j == n -1) {
				fprintf(fp, "%lf\n", G[i*n + j]);
			} else {
				fprintf(fp, "%lf, ", G[i*n + j]);
			}
		}
	}
		
	fclose(fp);
	free(G);
	cudaFree(G_D);
	exit(0);
}
