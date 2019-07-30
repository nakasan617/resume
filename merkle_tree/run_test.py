#!/usr/bin/env python3
import sys 
import subprocess 
import os 
import util 
import time

N = 4

def run_test(testcase): 
    string = "./each_test.sh " +  str(testcase)
    rv = subprocess.getoutput(string) 
    return rv 

def main():
    rv = util.make()
    if rv == -1:
        print("Make not successful") 
        return -1 

    for i in range(N):
        rv = run_test(i+1)
        if rv == "0":
            print("test " + str(i + 1) + " passed")
        else: 
            print("test " + str(i + 1) + " failed") 

    util.make_clean()
    return 0

main() 
