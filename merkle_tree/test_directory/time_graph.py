import matplotlib.pyplot as plt
import pandas as pd 
import numpy as np 
from decimal import Decimal 

def make_portion_graph(ifile, ofile, num):
    df = pd.read_csv(ifile, header = None)
    array = np.transpose(df.values).ravel()
    times = []
    for i in range(len(array)):
        times.append(np.array(array[i].split(':')))

    times = np.array(times) 
    #print(times) 
    
    # times[i][0] time to parse 
    # times[i][1] time to build tree 
    # times[i][2] time to hash 
    # times[i][3] time to hash for leaves
    # times[i][4] time to do everything
    abscissa = [] 
    for i in range(num):
        abscissa.append(3+i)


    to_parse = [] 
    to_build_tree = []
    to_hash = [] 
    to_hash_leaves = []
    to_all = []

    for i in range(len(array)): 
        if i == len(array) - 1:
            break
        to_parse.append(float(times[i][0]))
        to_build_tree.append(float(times[i][1])) 
        to_hash.append(float(times[i][2])) 
        to_hash_leaves.append(float(times[i][3])) 
        to_all.append(float(times[i][4])) 
 
    to_parse = np.array(to_parse) 
    to_build_tree = np.array(to_build_tree) 
    to_hash = np.array(to_hash) 
    to_hash_leaves = np.array(to_hash_leaves) 
    to_all = np.array(to_all) 

    # will make a graph that is composed of times to  
    # parse
    # to hash leaves 
    # to hash non-leaves 
    # to build trees others
    # others
   
    to_hash_others = to_hash - to_hash_leaves 
    to_build_tree_other = to_build_tree - to_hash 
    to_other = to_all - to_parse - to_build_tree 

    '''
    print("this is to_parse ", end='') 
    print(to_parse)
    print("this is to_hash_leaves ", end='') 
    print(to_hash_leaves) 
    print("this is to_hash_others ", end='') 
    print(to_hash_others) 
    print("this is to_build_tree_other ", end='') 
    print(to_build_tree_other) 
    print("this is to_other ", end='') 
    print(to_other)
    '''

    plt.clf()

    plt.bar(abscissa, to_other, width = 0.7, label = 'others', bottom = to_build_tree_other + to_hash_others + to_hash_leaves + to_parse) 
    plt.bar(abscissa, to_build_tree_other, width = 0.7, label = 'non-hashing part of building tree', bottom = to_hash_others + to_hash_leaves + to_parse)
    plt.bar(abscissa, to_hash_others, width = 0.7, label = 'hash non_leaves', bottom = to_hash_leaves + to_parse)
    plt.bar(abscissa, to_hash_leaves, width = 0.7, label = 'hash leaves', bottom = to_parse) 
    plt.bar(abscissa, to_parse, width = 0.7, label = 'parsing') 
    
    plt.ylabel('time (seconds)') 
    plt.xlabel('filesize (log10(bytes))')     
    plt.title('time spent (for each part) to compute merkle tree')
    plt.legend(loc = "upper left") 
    plt.show(block = False) 
    plt.pause(1) 
    plt.savefig(ofile, format = 'png')
    plt.close()

def make_each_graph(ifile, ofile, label):
    df = pd.read_csv(ifile, header = None)
    array = df.values
    array = np.transpose(array)
    array = np.ravel(array)
    # print(array) 

    size = [1000] 
    for i in range(len(array) - 1):
        size.append(10 * size[i]) 

    size = np.array(size) 
    size = np.log10(size) 
    
    plt.clf() 
    plt.plot(size, array)
    plt.xlabel('file size (log10(bytes))')
    plt.ylabel('time (seconds)')
    plt.title('time spent to make merkle tree ' + label + ' version') 
    plt.show(block = False)
    plt.pause(1) 
    plt.savefig(ofile, format = 'png') 
    plt.close()

def make_compare_graph(ifiles, ofile, labels):
    #ifiles is supposed to be the array of strings, each of which represents the input filename 
    plt.clf()

    for i in range(len(ifiles)):
        df = pd.read_csv(ifiles[i], header = None)
        array = df.values
        array = np.transpose(array)
        array = np.ravel(array)

        size = [1000] 
        for j in range(len(array) - 1):
            size.append(10 * size[j]) 

        size = np.array(size) 
        size = np.log10(size) 
        
        if len(size) > 5:
            size = size[:5]
            array = array[:5]
        
        plt.plot(size, array, label = labels[i])

    plt.legend()
    plt.xlabel('file size (log10(bytes))')
    plt.ylabel('time (seconds)')
    plt.title('time spent to make merkle tree compared') 
    plt.show(block = False)
    plt.pause(1) 
    plt.savefig(ofile, format = 'png') 
    plt.close()

    

def main():
    ifiles = ["time_create_merkle_txt.csv", "time_create_merkle_pdf.csv", "time_create_merkle_docx.csv", "time_create_merkle_NetCDF.csv", "time_create_merkle_h5.csv"]
    ofiles = ["time_merkle_tree_txt.png", "time_merkle_tree_pdf.png", "time_merkle_tree_docx.png", "time_merkle_tree_NetCDF.png", "time_merkle_tree_h5.png"]
    ofile = "time_merkle_tree_compare.png"
    labels = ["txt", "pdf", "docx", "NetCDF", "HDF5"]
    make_each_graph(ifiles[0], ofiles[0], labels[0])
    '''
    make_each_graph(ifiles[1], ofiles[1], labels[1]) 
    make_each_graph(ifiles[2], ofiles[2], labels[2]) 
    make_each_graph(ifiles[3], ofiles[3], labels[3]) 
    make_each_graph(ifiles[4], ofiles[4], labels[4]) 
    '''
   
    #make_compare_graph(ifiles[1:], ofile, labels[1:]) 
    
    ifile = "separate_create_merkle_txt.csv" 
    ofile = "time_portion_txt.png" 
    make_portion_graph(ifile, ofile, 8) 

    '''    
    ifile = "separate_create_merkle_binary_txt.csv" 
    ofile = "time_portion_txt_binary.png" 
    make_portion_graph(ifile, ofile, 8) 
 
    ifile = "separate_create_merkle_pdf.csv" 
    ofile = "time_portion_pdf.png" 
    make_portion_graph(ifile, ofile, 5) 

    ifile = "separate_create_merkle_docx.csv" 
    ofile = "time_portion_docx.png" 
    make_portion_graph(ifile, ofile, 5) 

    ifile = "separate_create_merkle_netcdf.csv" 
    ofile = "time_portion_netcdf.png" 
    make_portion_graph(ifile, ofile, 5) 

    ifile = "separate_create_merkle_hdf5.csv" 
    ofile = "time_portion_hdf5.png" 
    make_portion_graph(ifile, ofile, 8)  
    '''

main()
