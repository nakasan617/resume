from netCDF4 import Dataset 
import numpy as np 
import time

def one_kilo(): 
	dataset = Dataset('./one_kilo.nc', 'w', format='NETCDF4_CLASSIC') 
	levels = dataset.createDimension('level', 10) 
	lons = dataset.createDimension('lon', 10) 
	dataset.close()

def ten_kilo():
	dataset = Dataset('./ten_kilo.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(20):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def hundred_kilo():
	dataset = Dataset('./hundred_kilo.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(250):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def one_mega():
	dataset = Dataset('./one_mega.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(2500):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def func_thread(size, dataset):
	for i in size:
		string = str(i) 
		dataset.createDimension(string, 10) 

def ten_mega():	
	dataset = Dataset('./ten_mega.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(25000):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def m_ten_mega():
	dataset = Dataset('./ten_mega.nc', 'w', format='NETCDF4_CLASSIC') 
	jobs = []
	for i in xrange(0, threads):
		thread = Thread(target = func_thread, args = (chunk[i], dataset)) 
		jobs.append(thread) 
	
	# start the threads
	for j in jobs:
		j.start() 

	# ensure all of the threads have finished 
	for j in jobs:
		j.join()
	
def hundred_mega():
	dataset = Dataset('./hundred_mega.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(200000):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def one_giga():
	dataset = Dataset('./one_giga.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(2000000):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def ten_giga():
	dataset = Dataset('./ten_giga.nc', 'w', format='NETCDF4_CLASSIC') 
	for i in range(200000000):
		string = str(i) 
		string = dataset.createDimension(string, 10) 
	dataset.close()

def main():
	tic = time.clock()
	one_kilo()
	toc = time.clock() 
	print("one kilo took " + str(toc - tic) + " seconds")
	
	tic = time.clock()
	ten_kilo()
	toc = time.clock() 
	print("ten kilo took " + str(toc - tic) + " seconds")

	tic = time.clock()
	hundred_kilo()
	toc = time.clock() 
	print("hundred kilo took " + str(toc - tic) + " seconds")

	tic = time.clock()
	one_mega() 
	toc = time.clock() 
	print("one_mega took " + str(toc - tic) + " seconds") 

	tic = time.clock() 
	ten_mega() 
	toc = time.clock()
	print("ten_mega took " + str(toc - tic) + " seconds") 
	'''
	tic = time.clock()
	hundred_mega() 
	toc = time.clock()
	print*("hundred_mega took " + str(toc - tic) + " seconds") 
	'''

main()
