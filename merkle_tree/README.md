# WARNING

## WHEN YOU DO PREPARATORY WORK, THE MAKEFILE TRIES TO WRITE A FILE OF 10 GIGABYTE.
## MAKESURE THERE IS A DISK SPACE FOR THAT.

# WHAT EACH FILE DOES:
	control.sh:
		this file is to prepare all the files necessary, make the graphs which represents the performance of each type of file, tests all the testcases, and cleanup the files.
		the options are prepare, graph, test, clean. pass that as a first argument of the program.
 
	each_test.sh:
		runs each testcase in the directory, test_directory.

	compile.py: 
		compiles util.py and outputs util.pyc 

	run_test.py: (dependent on util.pyc) 
		runs all the testcases. 

	util.py: 
		library implementation for run_test.py 

	Makefile: 
		test: compiles all the testcases
		util: creates util.pyc 
		clean: deletes any object file, shared object file, executables  
	
	merkletree.h merkletree.c:
		header file and actual implementation of the library for merkletree

	parse.h parse.c:
		header file and actual implementation of the library to parse file and strings. 		This is just a library that is one level higher in abstraction than merkletree. 

	bfs.h bfs.c:
		header file and actual implementation of the library of breadth first search.

# TEST CASES
	Test cases are in the directory test_directory. 
## WHAT EACH TEST CASE CHECK 
### TEST1.C 
	This file test whether the basic operations of merkletree computation. 
	The test cases check the hash correctness for relatively large file (1 mega byte, function test_case1) and very small file (one line, function test_case2), what is checked is that whether the merkletree is built first, correctly hashes are recomputed.
### TEST2.C
	This file checks the size of the merkletree after the computation of the tree for files with various sizes. In particular, it checks whether the height of the tree and number of leaves are correct. 
### TEST3.C
	This file checks the computation of the merkle tree for the very large file (one giga byte). It checks whether the recomputation is correct when the file has been changed. 
### TEST4.C 
	This file checks whether time to recompute the node is larger than just checking whether the file has been tampered or not. 
### TEST5.C 
	Still in the process...
	 
# HOW TO TEST THE TESTCASES 	
	run 
		./control.sh prepare 
	to prepare all the files necessary to test. REMEMBER, THIS OPERATION WILL MAKE A VERY LARGE FILE IN THE DIRECTORY.
	run	
		./control.sh test
	to test all the testcases
	run 
		./control.sh clean
	to clean all the files that you made

# OTHER FUNCTIONALITIES
	you can make a graph for each filesize by typing the command
		./control.sh graph 
	after you run
		./control.sh prepare
	of course you can clean by 
		./control.sh clean

# PROSPECT
	I am trying to implement this to the version control system. 
