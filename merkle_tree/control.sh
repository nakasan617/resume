#!/bin/sh

preparation() {
	cd test_directory/example 

	# This will take around two minutes to complete
	# remember this will consume more than ten gigabytes of your disk. 
	make 
	./a.out
	cp ./one_giga.txt ./one_giga_small_change.txt
	echo 'a' >> ./one_giga_small_change.txt
	cd ../..
	python3 compile.py 
}

cleanup() {
	cd test_directory/example 
	make clean 
	cd ../..
	make clean
}

graph() {
	make graph 
	cd test_directory 
	./test_graph 
	python3 time_graph.py 
}

testcase() {
	python3 run_test.py 
}

if [ $1 = "prepare" ]; then 
	preparation 
	
	elif [ $1 = "clean" ]; then 
		cleanup 
	
	elif [ $1 = "graph" ]; then 
		graph 

	elif [ $1 = "test" ]; then 
		testcase

	else 
		echo "argument must be prepare, graph, test, or clean"
fi

