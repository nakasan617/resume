CC = gcc 
OPTS = -Wall 
SRCS = $(wildcard *.c) 
OBJS = $(patsubst %.c, %.o, $(SRCS))
SHOBJS = $(patsubst %.c, %.so, $(SRCS))
GRAPHS = $(wildcard test_directory/*.png)
CSVS = $(wildcard test_directory/*.csv)
LINKS = -lm -pthread -lssl -lcrypto
PROGS = bfs.c parse.c merkletree.c  
EXECS = test_directory/test1 test_directory/test2 test_directory/test3 test_directory/test4 test_directory/test_graph 

tests: test1 test2 test3 test4

test1: test_directory/test1.c  
	$(CC) $(OPTS) -std=c11 test_directory/test1.c $(PROGS) $(LINKS) -o test_directory/test1

test2: test_directory/test2.c  
	$(CC) $(OPTS) -std=c11 test_directory/test2.c $(PROGS) $(LINKS) -o test_directory/test2

test3: test_directory/test3.c  
	$(CC) $(OPTS) -std=c11 test_directory/test3.c $(PROGS) $(LINKS) -o test_directory/test3

test4: test_directory/test4.c 
	$(CC) $(OPTS) -std=c11 test_directory/test4.c $(PROGS) $(LINKS) -o test_directory/test4

graph: test_directory/test_graph.c
	$(CC) $(OPTS) -std=c11 test_directory/test_graph.c $(PROGS) $(LINKS) -o test_directory/test_graph  	

libraries: parse.so merkletree.so

objects: parse.o merkletree.o 

%.so: %.c 
	$(CC) $(OPTS) -std=c11 -shared -E -o $@ -fPIC $<

%.o: %.c
	$(CC) $(OPTS) -std=c11 -E -o $@ -c $< 

util: compile.py util.py 
	python compile.py 

.PHONY: clean
clean:  
	rm -f $(EXECS) util.pyc $(OBJS) $(SHOBJS) $(GRAPHS) $(CSVS) -r __pycache__ 
	 
	
