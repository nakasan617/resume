#include "../parse.h" 
#include "../merkletree.h"
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>

int test1() {
	merkle_tree mt;

	clock_t start = clock();
	FILE * fp = fopen("./example/one_giga.txt", "r");

	if(!fp) {
		perror("fp is null\n");
		return -1;
	}

	if(compute_merkle(&fp, &mt, 1) == -1) {
		perror("error in computing merkle\n");
		return -1;
	}
	clock_t end = clock();
	fclose(fp);

	double compute_merkle_time = (double)(end - start)/CLOCKS_PER_SEC;

	char path [] = "./example/one_giga_small_change.txt";
	
	start = clock();
	if(compare_recompute(&mt, path, RECOMPUTE) == -1) {
		perror("error in computing\n");
		return -1;
	}
	end = clock();
	
	double compare_recompute_time = (double)(end - start)/CLOCKS_PER_SEC;

	if(compare_recompute_time >= compute_merkle_time) {
		perror("why is it taking longer to recompute just one leaf than compute the whole merkle_tree?\n");
		return -1;
	}
 	
	free_merkle(&mt); 
	return 0;
}

int main() {
	if(test1() == -1) {
		printf("-1");
		return -1;
	}
	printf("0");
	return 0;
}
