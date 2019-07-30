from netCDF4 import Dataset 
import numpy as np
import time

def creating_variable(dataset):
	# create coordinate variables for 4-dimensions 
	times = dataset.createVariable('time', np.float64, ('time',)) 
	levels = dataset.createVariable('level', np.int32, ('level',))
	latitudees = dataset.createVariable('latitude', np.float32, ('lat',))
	longitudes = dataset.createVariable('longitude', np.float32, ('lon',)) 

	# Create the actual 4-d variable 
	temp = dataset.createVariable('temp', np.float32, ('time', 'level', 'lat', 'lon'))
	print('temp variable:'+ str(dataset.variables['temp']))
	for varname in dataset.variables.keys(): 
		var = dataset.variables[varname] 
		print(varname) 
		print(var.dtype)
		print(var.dimensions) 
		print(var.shape)  
		print()

def set_attributes(dataset):
	pass	

def main():
	dataset = Dataset('./test.nc', 'w', format='NETCDF4_CLASSIC')
	#print(dataset.file_format) 
	level = dataset.createDimension('level', 10)
	#lat = dataset.createDimension('lat', 73)
	#lon = dataset.createDimension('lon', 144)
	#time = dataset.createDimension('time', None)
	#print(len(lon)) 
	#print(time.isunlimited())
	'''
	print('Lon dimension:' + str(dataset.dimensions['lon']))
	for dimname in dataset.dimensions.keys():
		dim = dataset.dimensions[dimname]
		print(dimname, end = ' ')
		print(len(dim), end = ' ') 
		print(dim.isunlimited()) 
	creating_variable(dataset) 
	'''
	dataset.close() 
	
main()
