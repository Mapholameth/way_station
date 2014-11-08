import os
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
from random import random
from math import pi
import math
import csv

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from scipy.stats import multivariate_normal

from sample import *

a = [] # means
s = [] # covariance matrices
q = []

def classify(x):
    max_d = 0
    r_c = -1
    for i in range(m):
        d = multivariate_normal.pdf(x, mean=a[i], cov=s[i]) * q[i]
        if d > max_d:
            max_d = d
            r_c = i
    return r_c

learn_rate = 1.0

# http://upload.wikimedia.org/wikipedia/commons/e/ea/Anderson%27s_Iris_data_set.png
inputFilename = "data/iris.csv"

#inputFilename = "data/dogs&wolfs.csv"
dw_test = None
# dw_test = np.array([210,103,72,20.5,14.0,16.7])

sample_csv = SampleCSV.SampleCSV(inputFilename)
r, r_c = sample_csv.gen()

dim = sample_csv.dim_count
m = sample_csv.class_count

data = [[] for i in range(m)]

for i in range(len(r)):
    data[r_c[i]].append(r[i])

for j in range(m):
    a.append(np.zeros(dim))
    q.append(len(data[j]) / len(r))
    for i in range(int(len(data[j]) * learn_rate)):
        a[j] += data[j][i]
    a[j] /= len(data[j])
    print(a[j]) # means

    s.append(np.zeros((dim, dim)))

for j in range(m):
    for i in range(int(len(data[j]) * learn_rate)):
        x = data[j][i] - a[j]
        s[j] += x * x[np.newaxis, :].T

    s[j] /= len(data[j])
    print(s[j])

r_colors = ["r", "g", "b", "black"]
r_map = [[[] for j in range(dim)] for i in range(m + 1)]
print("test", r_map)

if dw_test != None:
    print(classify(dw_test))

wrong_cases = 0

for i in range(len(r)):
    classIndex = int(classify(r[i]))#r_c[i]#
    if classIndex != r_c[i]:
        classIndex = m
        wrong_cases += 1

    for j in range(dim):
        r_map[classIndex][j].append(r[i][j])

print("percent of errors", wrong_cases / len(r))

fig = plt.figure()

gs = gridspec.GridSpec(dim, dim)
for u in range(dim):
    for v in range(dim):
        ax = fig.add_subplot(gs[u, v])
        # ax.set_aspect(1)
        # ax.set_xlabel('X')
        # ax.set_ylabel('Y')
        for i in range(m + 1):
            alpha = 0.3
            if i == m:
                alpha = 1.0
            ax.scatter(r_map[i][u], r_map[i][v], color=r_colors[i], alpha=alpha)

plt.grid(True)
plt.show()
plt.savefig("figure.png", dpi=300)