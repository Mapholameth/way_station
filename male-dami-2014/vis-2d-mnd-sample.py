import os
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

import numpy as np
import matplotlib.pyplot as plt

from sample import *

if __name__ == "__main__":
    sample_mnd = SampleMND.SampleMND(
        2, 3,
        np.array([
            [0, 0],
            [0, 4],
            [-6, 2]
        ]),
        [
            0.8,
            0.1,
            0.1
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

    r, r_c = sample_mnd.gen(500);
    r_colors = ["r", "g", "b"]

    for i in range(len(r)):
        plt.scatter(r[i][0], r[i][1], alpha=0.3, color=r_colors[r_c[i]])

    plt.grid(True)
    plt.show()