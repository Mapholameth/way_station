from pybrain.tools.shortcuts import buildNetwork
from pybrain.structure.modules import TanhLayer, SoftmaxLayer, GaussianLayer,\
    SigmoidLayer, LinearLayer, GateLayer
from pybrain.datasets import SupervisedDataSet

# net = buildNetwork(1, 100, 50, 25, 1, hiddenclass=TanhLayer)
net = buildNetwork(1, 8, 3, 1, hiddenclass=SigmoidLayer) # TanhLayer


ds = SupervisedDataSet(1, 1)

from math import exp

def f(x):
    return (1 - 0.1 * x) +\
    (-0.5 + 1 / (1 + exp(-75-40*x))) +\
    (-0.5 + 1 / (1 + exp(-2*x))) +\
    (-1 + 1 / (1 + exp(-170+85*x)))
    
import matplotlib.pyplot as plt

x = []
y = []
y_n = []

n = 32

for i in range(n):
    arg = (i/n)*6 - 3
    x.append(arg)
    y.append(f(arg))
    ds.addSample((arg), (f(arg)))

from pybrain.supervised.trainers import BackpropTrainer
from pybrain.supervised.trainers import RPropMinusTrainer

trainer = BackpropTrainer(net, ds, learningrate=0.1)
# trainer = RPropMinusTrainer(net, ds)

# trainer.trainUntilConvergence()
for i in range(10):
     trainer.train()

for i in range(n):
    arg = (i/n)*6 - 3
    y_n.append(net.activate([arg]))
    

fig = plt.figure()
plt.plot(x, y, 'r')
plt.plot(x, y_n, 'b')

plt.show()

# x in [-3; 3]