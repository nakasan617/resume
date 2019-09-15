#!/usr/bin/env python
# coding: utf-8

import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt

def show_heatmap(path, title, filename):
    df = pd.read_csv(path, header = None)
    # I need to standardize the values
    array = df.values
    plt.clf()
    plt.title(title)
    plt.imshow(array)
    plt.show(block = False) 
    plt.pause(2) 
    plt.savefig(filename)

path = "out.csv"
show_heatmap(path, "Ray-Traced Render of a Sphere Illuminated from Top Left", "parallel.png")






