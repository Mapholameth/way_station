from math import exp
from random import random

from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure.modules import TanhLayer, SoftmaxLayer, GaussianLayer,\
    SigmoidLayer, LinearLayer, GateLayer
from pybrain.datasets import SupervisedDataSet
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.supervised.trainers import RPropMinusTrainer

import matplotlib.pyplot as plt

# с методом обучения back prop сходится быстро, но слишком точно повторяет шум
# 100 50 25 нейронов в скрытых слоях
net_big = buildNetwork(1, 100, 50, 25, 1, hiddenclass=TanhLayer)

# 8 3 нейронов в скрытых слоях, плохо обучается back prop, хорошо обучается
# RProp-, cf. [Igel&Huesken, Neurocomputing 50, 2003
net = buildNetwork(1, 8, 3, 1, hiddenclass=TanhLayer)


ds = SupervisedDataSet(1, 1)

def f(x):
    return (1 - 0.1 * x) +\
    (-0.5 + 1 / (1 + exp(-75-40*x))) +\
    (-0.5 + 1 / (1 + exp(-2*x))) +\
    (-1 + 1 / (1 + exp(-170+85*x)))

x = []
y = []
y_noise = []
y_n = []
y_n_big = []

n = 128

for i in range(n):
    arg = (i/n)*6 - 3
    x.append(arg)
    r = f(arg) + (random() - 0.5) * 0.2
    y.append(f(arg))
    y_noise.append(r)
    ds.addSample((arg), (r))

trainer_big = BackpropTrainer(net_big, ds, learningrate=0.01, lrdecay=1.0, momentum=0.0, weightdecay=0.0)

          #  RProp-, cf. [Igel&Huesken, Neurocomputing 50, 2003
trainer = RPropMinusTrainer(net, dataset=ds)

# trainer.trainUntilConvergence()

for i in range(100):
     trainer.train()

for i in range(10):
    trainer_big.train()

for i in range(n):
    arg = (i/n)*6 - 3
    y_n.append(net.activate([arg]))
    y_n_big.append(net_big.activate([arg]))



fig = plt.figure()
plt.plot(x, y, 'black')
plt.plot(x, y_noise, 'r')
plt.plot(x, y_n, 'b')
plt.plot(x, y_n_big, 'g')

plt.show()

# x in [-3; 3]
