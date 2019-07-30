/////////////////////////////////////////////////////////////////////////////////////////////////////
WARNING

WHEN YOU DO PREPARATORY WORK, THE MAKEFILE TRIES TO WRITE A FILE OF 10 GIGABYTE.
MAKESURE THERE IS A DISK SPACE FOR THAT.

/////////////////////////////////////////////////////////////////////////////////////////////////////


WHAT EACH FILE DOES:
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

