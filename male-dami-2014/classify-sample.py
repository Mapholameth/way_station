import os
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
from random import random
from math import pi
import math

import numpy as np
import matplotlib.pyplot as plt

from sample import *

m = 3
dim_count = 2

a = np.array([
    [0, 0],
    [0, 3],
    [-8, 1]
])

q = [
    0.8,
    0.1,
    0.1
]

s = np.array([
    [
        [1, 0.5],
        [-0.5, 1],
    ],
    [
        [0.15, 0.25],
        [-0.25, 0.5],
    ],
    [
        [4, 0.5],
        [-0.5, 1],
    ],
])

from scipy.stats import multivariate_normal

def classify(x):
    max_d = 0
    r_c = -1
    for i in range(m):
        d = multivariate_normal.pdf(x, mean=a[i], cov=s[i]) * q[i]
        if d > max_d:
            max_d = d
            r_c = i
    return r_c

sample_mnd = SampleMND.SampleMND(dim_count, m, a, q, s)

r, r_c = sample_mnd.gen(2000)
r_colors = ["r", "g", "b", "black"]
r_map = []
for i in range(m + 1):
    t = [[],[]]
    r_map.append(t)

for i in range(len(r)):
    classIndex = classify(r[i])
    if classIndex != r_c[i]:
        classIndex = 3
    r_map[classIndex][0].append(r[i][0])
    r_map[classIndex][1].append(r[i][1])

for i in range(m + 1):
    plt.scatter(r_map[i][0], r_map[i][1], color=r_colors[i], alpha=0.3)

plt.grid(True)
plt.show()