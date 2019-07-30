#include "../parse.h"
#include "../merkle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int test1() {
	FILE *fp = fopen("./example/hundred_mega.txt", "r");
	if(!fp) {

	int file_size = get_file_size(&fp);
	
	int nb_of_pages = (int)(file_size / PAGE_LENGTH);
	int nb_of_leaves = count_nb_of_leaves(nb_of_pages);
	if(file_size % PAGE_LENGTH != 0) {
		nb_of_pages++;
	}
	 
	clock_t start = clock();
	clock_t end = clock();
	double parse_time_single = (double)(end - start)/CLOCKS_PER_SEC;	
	return 0;
}
		
int main() {
	if(test1() == -1) {
		return EXIT_FAILURE;
	}
	printf("0");
	return EXIT_SUCCESS;	
}
