#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#undef __STRICT_ANSI__
#include <math.h>
//#include <cuda.h>
#include <stdint.h> 
#include "my_vector.h"
//#include "mtwister.h"
//#include <time.h> 
#include <omp.h>

#define W_MAX 10

double ** allocate_G(int n) {
	double ** rv = (double **)calloc(n, sizeof(double *));
	int i = 0;
	for(i = 0; i < n; i++) {
		rv[i] = (double *)calloc(n, sizeof(double));
	}
	return rv;
}

double LCG_random_double(uint64_t * seed) {
	const uint64_t m = 9223372036854775808ULL;
	const uint64_t a = 2806196910506780709ULL;
	const uint64_t c = 1ULL;
	*seed = (a* (*seed) + c) % m;
	return (double) (*seed)/ (double)m;
}


int translation(double a, double start, double end, double N, int axis) {
	//printf("this is a %lf\n", a);
	double cell_size = (end - start)/N;
	//printf("this is cell_size %lf\n", cell_size);
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
		//printf("this is start %lf\n", start);
	} else {
		while(!(start >= a && a > start - cell_size)) {
			rv++;
			start -= cell_size;
			assert(start >= end);
		} 
	}
	return rv;
}

static inline int condition(double W_x, double W_z, vector * V, vector * C, double R) {
	//printf("this is W_x %lf, W_z %lf \n", W_x, W_z);
	if( (-1 * W_MAX < W_x) && (W_x < W_MAX) && (-1 * W_MAX < W_z) && (W_z < W_MAX)) {
		double inner = vector_inner(V, C);
		double norm = vector_inner(C, C);
		//printf("this is condition %lf\n", inner * inner + R * R - norm);
		if( inner * inner + R * R - norm > 0) {
			return 1; 
		}
		return 0;
	}
	return 0;
}

int main(int argc, char ** argv) {
	assert(argc == 3);
	int n = atoi(argv[1]);
	int N_rays = atoi(argv[2]);
	
	/*
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start);
	*/
	// I should be hardcoding the below
	//clock_t tic = clock();
	double tic = omp_get_wtime();
	double W_y = 10;
	double R = 6;
	double * L_inside = (double *)malloc(sizeof(double) * 3);
	double * C_inside = (double *)malloc(sizeof(double) * 3);
	L_inside[0] = 4; L_inside[1] = 4; L_inside[2] = -1;
	C_inside[0] = 0; C_inside[1] = 12; C_inside[2] = 0;
	vector * L = vector_create(3, L_inside);
	vector * C = vector_create(3, C_inside);
	free(L_inside);
	free(C_inside);
	assert(L->content != NULL);
	assert(C->content != NULL);
	uint64_t seed = 4238811;

	//MTRand r = seedRand(1337);

	double ** G = allocate_G(n);
	assert(G[n-1][n-1] == 0);

	double phi = -1;
	double cos_theta = -2;
	double sin_theta = -2;
	double * Vs = (double *)calloc(3, sizeof(double));
	vector * V = NULL; V = vector_create_space(V, 3);
	vector * W = NULL; W = vector_create_space(W, 3);
	vector * I = NULL; I = vector_create_space(I, 3);
	vector * N = NULL; N = vector_create_space(N, 3);
	vector * S = NULL; S = vector_create_space(S, 3);

	double W_x = -1;
	double W_z = -1;
	double t = 0;
	double b = 0;
	int i = 0;
	int j = 0;

	int k = 0;	
	for(k = 0; k < N_rays; k++) {
		// sampling V
		do { 
			phi = 2 * M_PI * LCG_random_double(&seed); // this should be 0 to 2 pi 			
			//printf("this is phi %lf\n", phi);
			
			cos_theta = 2 * LCG_random_double(&seed) - 1; // this should be -1 to 1
			//printf("this is cos_theta %lf\n", cos_theta);
			
			sin_theta = sqrt( 1 - cos_theta * cos_theta);
			assert(0 < phi && phi < 2 * M_PI); assert(-1 <= cos_theta && cos_theta <= 1); assert(-1 <= sin_theta && sin_theta <= 1);
			Vs[0] = sin_theta * cos(phi); Vs[1] = sin_theta * sin(phi); Vs[2] = cos_theta; 
			V = vector_content_copy(V, Vs, 3);
			W = vector_scalar_mult(V, W, W_y/Vs[1]);
			W_x = W->content[0];
			W->content[1] = W_y;
			W_z = W->content[2];
		} while (condition(W_x, W_z, V, C, R) == 0);
		double a = vector_inner(V, C);
		t = a - sqrt(a * a + R * R - vector_inner(C, C));
		I = vector_scalar_mult(V, I, t);

		// calculating N here 
		N = vector_subtraction(I, C, N);
		a = sqrt(vector_inner(N, N));
		assert(a != 0);
		N = vector_scalar_mult(N, N, 1/a);

		// calculating S here
		S = vector_subtraction(L, I, S);
		a = sqrt(vector_inner(S, S));
		assert(a != 0);
		S = vector_scalar_mult(S, S, 1/a);

		// change b accordingly
		b = vector_inner(S, N);
		if(b < 0) b = 0;
		//printf("this is b %lf\n", b);
		//printf("this is W_x, W_z, %lf, %lf\n", W_x, W_z);

		i = translation(W_x, W_MAX, -1 * W_MAX, n, 1);
		j = translation(W_z, -1 * W_MAX, W_MAX, n, 0);
		//printf("this is i, j %d, %d\n", i, j);
		G[i][j] += b;		
		//print_vector(W);
	}
	//clock_t toc = clock();
	double toc = omp_get_wtime();
	double seconds = toc - tic;
	printf("it took %lf seconds to iterate through %d rays\n", seconds, N_rays);
	/*
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);

	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("it took %f microseconds when %d number of rays are used\n", milliseconds, N_rays);
	*/
	// print to out.csv
	FILE * fp = fopen("out.csv", "w");
	//fp = stdout;
	int alpha = 0;
	int beta = 0;
	for(alpha = 0; alpha < n; alpha++) {
		for(beta = 0; beta < n; beta++) {
			if( beta == n -1) {
				fprintf(fp, "%lf\n", G[alpha][beta]);
			} else {
				fprintf(fp, "%lf, ", G[alpha][beta]);
			}
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
