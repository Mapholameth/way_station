import os
import sys
import random
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

import numpy as np
import matplotlib.pyplot as plt

from sample import *

d = 2
k = 3

sample_mnd = SampleMND.SampleMND(
    d, k,
    np.array([
        [0, 0],
        [0, 4],
        [-6, 2]
    ]),
    [
        0.7,
        0.1,
        0.2
    ],
    np.array([
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
    ]))

points, r_classes_ref = sample_mnd.gen(500)
r_classes = [-1 for i in range(len(points))]

min_e = [9999 for i in range(d)]
max_e = [-9999 for i in range(d)]

for i in range(len(points)):
    for j in range(d):
        if points[i][j] < min_e[j]:
            min_e[j] = points[i][j]
        if points[i][j] > max_e[j]:
            max_e[j] = points[i][j]

means = []
for i in range(k):
    means.append([min_e[j] + random.random()*(max_e[j] - min_e[j]) for j in range(d)])

means_changed = True

def calc_dist2(a, b):
    r = 0
    for i in range(d):
        r += (a[i] - b[i])**2
    return r

while (means_changed):
    means_changed = False
    new_means = [0 for i in range(k)]
    new_means_size = [0 for i in range(k)]
    for i in range(len(points)):
        dist = 999999
        new_class = -1
        for j in range(k):
            dist2 = calc_dist2(means[j], points[i])
            if dist2 < dist:
                new_class = j
                dist = dist2
        if new_class != r_classes[i]:
            r_classes[i] = new_class
            means_changed = True

        new_means[new_class] += points[i]
        new_means_size[new_class] += 1

    for i in range(k):
        means[i] = new_means[i] / new_means_size[i]

    print(means)

r_colors = ["r", "g", "b", "y"]
r_map = []
for i in range(k + 1):
    t = [[],[]]
    r_map.append(t)

for i in range(len(points)):
    classIndex = r_classes[i]
    # if classIndex != r_classes_ref[i]:
    #     classIndex = 3
    r_map[classIndex][0].append(points[i][0])
    r_map[classIndex][1].append(points[i][1])

for i in range(k + 1):
    plt.scatter(r_map[i][0], r_map[i][1], color=r_colors[i], alpha=0.3)

plt.grid(True)
plt.show()