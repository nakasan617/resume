#include "../parse.h"
#include "../merkletree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

int test1(merkle_tree * mt) {
	FILE * fp = fopen("./example/one_giga.txt", "r");
	if(!fp) {
		perror("fp is null\n");
		return -1;
	}
	
	if(compute_merkle(&fp, mt, 1) == -1) {
		perror("error in computing merkle\n");
		return -1;
	}

	char path [] = "./example/one_giga.txt";
	// use the same file and see if they return 0
	int rv = -1;
	if((rv = compare_recompute(mt, path, DONT)) != 0) {
		fprintf(stderr, "they are the same file so it should return 0, but it is returning %d\n", rv);
		
		return -1;
	}
	
	return 0;
}

int test2(merkle_tree * mt) {
	// compare the time to recompute and not recompute, the time not to recompute must be faster
	char path [] = "./example/one_giga_small_change.txt";
	int rv = -1;

	// the time without computing is measured.
	clock_t start = clock();
	if((rv = compare_recompute(mt, path, DONT)) <= 0) {
		perror("compare and recompute should return value bigger than 0\n");
		return -1;
	}
	clock_t end = clock();
	double no_recompute = (double)(end - start)/CLOCKS_PER_SEC;

	// the time with computing is measured. 
	start = clock();
	if((rv = compare_recompute(mt, path, RECOMPUTE)) <= 0) {
		perror("compare and recompute should return value bigger than 0\n");
		return -1;
	}
	end = clock();
	double recomputed = (double)(end - start)/CLOCKS_PER_SEC;

	if(recomputed < no_recompute) {
		perror("the time without recomputation of the nodes must be faster\n");
		fprintf(stderr,"this is the time without recomputation %lf\n", no_recompute);
		fprintf(stderr, "this is the time with recomputation %lf\n", recomputed);
		return -1;
	}

	//printf("print how much time it was spent to recompute %lf\n", recomputed);
	//printf("print how much time it was spent to just to compare %lf\n", no_recompute);
	return 0;
}

int main() {
	merkle_tree mt;
	if(test1(&mt) == -1) {
		free_merkle(&mt);
		//printf("failed test1\n");	
		return -1;
	}

	if(test2(&mt) == -1) {
		free_merkle(&mt);
		//printf("failed test2\n");
		return -1;
	}
	
	printf("0");
	free_merkle(&mt);
	return 0;
}
