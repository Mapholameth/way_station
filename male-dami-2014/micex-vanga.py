import random
import csv

from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure import RecurrentNetwork
from pybrain.structure.modules import TanhLayer, SoftmaxLayer, GaussianLayer,\
    SigmoidLayer, LinearLayer, GateLayer
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.supervised.trainers import RPropMinusTrainer

import numpy as np
import matplotlib.pyplot as plt

# <DATE>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>

# GAZP_131101_141031.txt
# GMKN_131101_141031.txt
# LKOH_131101_141031.txt
# MGNT_131101_141031.txt
# ROSN_131101_141031.txt
# SBER_131101_141031.txt
# SNGS_131101_141031.txt
# URKA_131101_141031.txt
# VTBR_131101_141031.txt

csv_file = open('data/GAZP_131101_141031.txt')
reader = csv.reader(csv_file, dialect="excel")

x = []
openPrice = []
maxPrice = []
minPrice = []
closePrice = []
volumeSold = []

for row in reader:
  x.append(row[0] + row[1])
  openPrice.append(row[2])
  maxPrice.append(row[3])
  minPrice.append(row[4])
  closePrice.append(row[5])
  volumeSold.append(row[6])

windowSize = 9 * 4

net = buildNetwork(windowSize, 8, 4, 2, 1, hiddenclass=SigmoidLayer) 
ds = SupervisedDataSet(windowSize, 1)
# , recurrent=True

n = windowSize * 8

for i in range(n):
    if i >= windowSize:
      ds.addSample(([closePrice[i - (windowSize - j)] for j in range(windowSize)]), (closePrice[i]))

trainer = BackpropTrainer(net, ds) #, learningrate=0.01, lrdecay=1.0, momentum=0.0, weightdecay=0.0)
# trainer = RPropMinusTrainer(net, dataset=ds)
for i in range(100):
    print('error: ', trainer.train(), '\n')

# print('error: ', trainer.trainUntilConvergence())

y = []
y_n = []

for i in range(n):
    y.append(closePrice[i])
    if i < windowSize:
      y_n.append(closePrice[i])
    else:
      r = net.activate([closePrice[i - (windowSize - j)] for j in range(windowSize)])
      y_n.append(r[0])

fig = plt.figure()
# plt.plot(maxPrice, '')
# plt.plot(minPrice, '')
# plt.plot(openPrice, '')
plt.plot(y, 'b')
plt.plot(y_n, 'r')
plt.show()
