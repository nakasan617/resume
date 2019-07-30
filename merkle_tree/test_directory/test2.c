#include "../parse.h" 
#include "../merkletree.h" 
#include <stdio.h> 
#include <stdlib.h>

// remember that page length is 65536 bytes 
// I need to check whether we can get it right tree size 
// let's first do it for the text files
int test1() {
	FILE * fp = fopen("./example/one_mega.txt", "r");
	if(!fp) {
		perror("fp is null\n");
		return -1;
	}	
	
	merkle_tree mt_a;
	if(compute_merkle(&fp, &mt_a, 1) == -1) {
		perror("error in computing merkle\n");
		return -1;
	}

	if(get_tree_height(&mt_a) != 5) {
		perror("tree height not correct\n");
		return -1;
	}

	if(get_nb_leaves(&mt_a) != 16) {
		perror("nb_of_leaves not correct\n");
		return -1;
	}

	free_merkle(&mt_a);
	fclose(fp);

	fp = fopen("./example/hundred_mega.txt", "r");
	if(!fp) {
		perror("fp is null\n");
		return -1;
	}	
	
	merkle_tree mt_b;
	if(compute_merkle(&fp, &mt_b, 1) == -1) {
		perror("error in computing merkle\n");
		return -1;
	}

	int rv = -1;
	if((rv = get_tree_height(&mt_b)) != 12) {
		perror("tree height not correct\n");
		return -1;
	}

	if((rv = get_nb_leaves(&mt_b)) != 2048) {
		perror("nb_of_leaves not correct\n");
		return -1;
	}
	//printf("%d\n", rv);

	free_merkle(&mt_b);
	fclose(fp);

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
