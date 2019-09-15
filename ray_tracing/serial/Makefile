main: main.c my_vector.h 
	gcc -Wall -std=c11 -U__STRICT_ANSI__ main.c -o main -lm -fopenmp 

time: main
	./main 100 1280
	./main 100 12800
	./main 100 128000
	./main 100 1280000
	./main 100 12800000
	./main 100 128000000

.PHONY: clean
clean: 
	rm -f main 
