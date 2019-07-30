import numpy as np 
import h5py 
import time 

def one_kilo():
	hf = h5py.File('one_kilo.h5', 'w') 
	hf.close()

def ten_kilo():
	d1 = np.random.random(size = (30, 33)) 

	hf = h5py.File('ten_kilo.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def hundred_kilo():
	d1 = np.random.random(size = (1250, 10)) 
	hf = h5py.File('hundred_kilo.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def one_mega():
	d1 = np.random.random(size = (1250, 100)) 
	hf = h5py.File('one_mega.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def ten_mega():
	d1 = np.random.random(size = (1250, 1000)) 
	hf = h5py.File('ten_mega.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def hundred_mega():
	d1 = np.random.random(size = (1250, 10000)) 
	hf = h5py.File('hundred_mega.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def one_giga():
	d1 = np.random.random(size = (1250, 100000)) 
	hf = h5py.File('one_giga.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def ten_giga():
	d1 = np.random.random(size = (1250, 1000000)) 
	hf = h5py.File('ten_giga.h5', 'w') 
	hf.create_dataset('dataset_1', data= d1) 
	hf.close()

def main():
	tic = time.time()
	one_kilo()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make one_kilo")

	tic = time.time()
	ten_kilo()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make ten_kilo")

	tic = time.time()
	hundred_kilo()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make hundred_kilo")

	tic = time.time()
	one_mega()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make one_mega")

	tic = time.time()
	ten_mega()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make ten_mega")

	tic = time.time()
	hundred_mega()	
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make hundred_mega")

	tic = time.time()
	one_giga()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make one_giga")

	tic = time.time()
	ten_giga()
	toc = time.time() 
	print("it took " + str(toc - tic) + " seconds to make ten_giga")

main()
