
threads: main
	./main 100 100 256
	./main 100 200 128
	./main 100 400 64
	./main 100 800 32
	./main 100 1600 16 
	./main 100 3200 8
	./main 100 6400 4
	
comparison: main 
	./main 100 10 128
	./main 100 100 128
	./main 100 1000 128
	./main 100 10000 128
	./main 100 100000 128
	./main 100 1000000 128

image: main
	./main 1000 10000000 100

main: main.cu 
	nvcc -U__STRICT_ANSI__ main.cu -o main -lm -arch=sm_37

.PHONY: clean 
clean: 
	rm -f main
