import csv
import random

import numpy as np

class SampleCSV:
    """Read data from csv file and transform it
    """
    def __init__(self, filename):
        csv_file = open(filename)

        reader = csv.reader(open(filename), dialect="excel")

        self.class_count = 0
        self.class_names = {}
        self.classes = []
        self.points = []

        for row in reader:
            l = len(row)
            self.dim_count = l - 1
            x = np.zeros(l - 1)
            for i in range(l):
                if i < l - 1:
                    x[i] = float(row[i])
                else:
                    key = row[i]
                    if key not in self.class_names:
                        self.class_names[key] = self.class_count
                        self.class_count += 1
                    self.classes.append(self.class_names[key])
                    self.points.append(x)

    def gen(self):
        l = len(self.points)
        data = [[self.points[i], self.classes[i]] for i in range(l)]
        random.shuffle(data)
        r = [data[i][0] for i in range(l)]
        r_classes = [data[i][1] for i in range(l)]

        return (r, r_classes)