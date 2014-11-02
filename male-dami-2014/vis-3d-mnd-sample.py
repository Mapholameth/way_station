import os
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from sample import *

if __name__ == "__main__":
    sample_mnd = SampleMND.SampleMND(
        3, 3,
        np.array([
            [0, 0, 0],
            [4, 4, 0],
            [-6, 2, 0]
        ]),
        [
            0.8,
            0.1,
            0.1
        ],
        np.array([
            [
                [1.0, 0.5, 0.2],
                [0.5, 1.0, 0.2],
                [0.2, 0.2, 1.0],
            ],
            [
                [1.0, 0.5, 0.2],
                [0.5, 1.0, 0.2],
                [0.2, 0.2, 1.0],
            ],
            [
                [1.0, 0.5, 0.2],
                [0.5, 1.0, 0.2],
                [0.2, 0.2, 1.0],
            ],
        ]))

    r, r_c = sample_mnd.gen(500);
    r_colors = ["r", "g", "b"]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    for i in range(len(r)):
        ax.scatter3D(r[i][0], r[i][1], r[i][2], alpha=0.3, color=r_colors[r_c[i]])

    plt.grid(True)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()