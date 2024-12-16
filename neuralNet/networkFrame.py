import numpy as np

class net:
    def __init__(self,layers):
        # creates an array of layer objects
        self.net = np.empty(len(layers),dtype=layer)
        for x in range(len(layers)):
            self.net[x] = layer(layers[x-1],layers[x])
        
    def getBias(self,layer):
        if layer == 0:
            print("input layer contains no Bias")
        else:
            print(self.net[layer].bias)
    
    def getWeights(self,layer):
        if layer == 0:
            print("input layer contains no Weights")
        else:
            print(self.net[layer].weights)
    
class layer:
    def __init__(self,prevsize,size):
        self.bias = np.random.randn(size).reshape(-1,1)
        self.weights = np.random.randn(size, prevsize)
        self.sum = np.empty(size, dtype=float)
        self.activations = np.empty(size, dtype=float)
        self.errors = np.empty(size,dtype = float)
        self.weightErrors = np.empty((size, prevsize),dtype = float)
        self.biasErrors = np.empty(size,dtype = float).reshape(-1,1)
        
       





