#!/usr/bin/env python

#import commands
import subprocess
import os
import time


testdir = "./testcases"

# --- make everything
def make():
    """make sure a make file exists, and make generate server."""
    if not (os.path.exists("Makefile") or os.path.exists("makefile")):
        print("ERROR: missing Makefile or makefile")
        return -1

    subprocess.getoutput("make clean")

    subprocess.getoutput("make")
    if not os.path.exists("./test_directory/test1"):
        print("ERROR: make does not generate an excutable server file")
        return -1
    return 0

# --- clean everything 

def make_clean():
    subprocess.getoutput("make clean") 
    return 0

