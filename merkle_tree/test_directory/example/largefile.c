#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
//#define FILE_LENGTH 1000
//#define STR_LENGTH 1000
#define NUM_THREADS 4
#define SCHEDULE guided 
#define BLOCK_SIZE 1

// this function is taken from https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
char *randstring(int length) {
	static int mySeed = 25011984;
	char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    	size_t stringLen = strlen(string);        
	char *randomString = NULL;

	srand(time(NULL) * length + ++mySeed);

	if (length < 1) {
		length = 1;
	}

    	randomString = malloc(sizeof(char) * (length +1));

	if (randomString) {
		short key = 0;

		for (int n = 0;n < length;n++) {            
			key = rand() % stringLen;          
			randomString[n] = string[key];
		}

		randomString[length] = '\0';

		return randomString;        
	}
	else {
		printf("No memory");
		exit(1);
	}
}

int write_random_files(char path [], int file_length, int str_length) {
	//omp_set_num_threads(NUM_THREADS);
	FILE * fd = fopen(path, "w");
	if(!fd) {
		perror("failed to open the file\n");
		return -1;
	}
	clock_t start = clock();
	//for(int i = 0; i < FILE_LENGTH ; i++) {
	//#pragma omp parallel for schedule(SCHEDULE, BLOCK_SIZE) default(none) shared(file_length, str_length, fd) private()
	for(int i = 0; i < file_length; i++) {
		char * random_string = randstring(str_length);
		fwrite((void *)random_string, str_length, 1, fd);
		if(random_string) {
			free(random_string);
		}
	}
	clock_t end = clock();
	fclose(fd);

	printf("it took %lf seconds to make %s\n", ((double)(end - start))/CLOCKS_PER_SEC, path);
	return 0;
}

int main(int argc, char *argv []) {
	// let's fix the string_length and alternate the file_length
	int str_length = 1000;
	
	// one kilo byte 
	if(write_random_files("one_kilo.txt", 1, str_length) == -1) {
		perror("writing one kilo failed\n");
		return EXIT_FAILURE;
	}

	// ten kilo byte
	if(write_random_files("ten_kilo.txt", 10, str_length) == -1) {
		perror("writing ten kilo failed\n");
		return EXIT_FAILURE;
	}
	
	// hundred kilo byte
	if(write_random_files("hundred_kilo.txt", 100, str_length) == -1) {
		perror("writing hundred kilo failed\n");
		return EXIT_FAILURE;
	}

	// one mega byte
	if(write_random_files("one_mega.txt", 1000, str_length) == -1) {
		perror("writing one mega failed\n");
		return EXIT_FAILURE;
	}

	// ten mega byte 
	if(write_random_files("ten_mega.txt", 10000, str_length) == -1) {
		perror("writing ten mega failed\n");
		return EXIT_FAILURE;
	}

	// hundred mega byte
	if(write_random_files("hundred_mega.txt", 100000, str_length) == -1) {
		perror("writing hundred mega failed\n");
		return EXIT_FAILURE;
	}

	// one giga byte 
	if(write_random_files("one_giga.txt", 1000000, str_length) == -1) {
		perror("writing one giga failed\n");
		return EXIT_FAILURE;
	}

	// ten giga byte
	if(write_random_files("ten_giga.txt", 10000000, str_length) == -1) {
		perror("writing ten giga failed\n");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
} 
