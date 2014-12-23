from random import random

from scipy.stats import norm
import numpy as np
from numpy.linalg import det
from numpy.linalg import cholesky
import matplotlib.pyplot as plt

class SampleMND:
    """MND - Multivariate Normal Distribution
    """
    def __init__(self, dim_count, class_count, a, q, s):
        self.dim_count = dim_count
        self.class_count = class_count
        self.a = a

        self.q = []
        t = 0
        for i in range(class_count):
            t += q[i]
            self.q.append(t)

        self.s = s

    def gen(self, n):
        def sel_i():
            i = random()
            for j in range(self.class_count):
                if i < self.q[j]:
                    return j
            return self.class_count - 1

        r = []
        r_classes = []

        for j in range(n):
            i = sel_i()
            x = np.zeros(self.dim_count)
            for k in range(self.dim_count):
                x[k] = norm.ppf(random())
            r.append(cholesky(self.s[i]).dot(x) + self.a[i])
            r_classes.append(i)

        return (r, r_classes)
