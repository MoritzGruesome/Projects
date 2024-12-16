
import numpy as np
import networkFrame as frame
import MNIST as nist

#contains functions for foreward propagation
class ForewardProp:

    #calculates the weighted sum of current layer based on previous layer
    #prev and current are layer objects l-1 and l respectively
    def propagate(self,prev,current):
        #print(str(np.matmul(current.weights,prev.activations).shape))
        result = np.add(np.matmul(current.weights,prev.activations),current.bias)
        return result
    
       
    #component wise function, applies the sigmoid function to each element in the matrix
    @staticmethod
    def sigmoid(z):
        z_clipped = np.clip(z, -800, 800)
        return 1.0/(1.0+np.exp(-z_clipped))
    
    #takes a network of class network and one training example
    #calculates the sums and activations of each neuron in netowork through foreward propagation
    #returns a copy of network with updated activations and sums
    def forwardprop(self,network,example):
        if len(example) != len(network[0].bias):
            raise Exception("size of example must equal size of input layer")
        network[0].activations = example.reshape(-1,1)
        for l in range(1,len(network)):
            network[l].sum = self.propagate(network[l-1],network[l])
            network[l].activations = self.sigmoid(network[l].sum)
        return network
    
    def getPredictedNum(self,network,index,nist):
        net = self.forwardprop(network,nist.TestData[index].trainingData)
        output = net[-1].activations
        cur = 0
        for o in range(len(output)):
            if output[o] > output[cur]:
                cur = o
        if cur == nist.TestData[index].label:
            return True
    
    def PredictNumFromGrid(self,network,normalGrid):
        net = self.forwardprop(network,normalGrid)
        output = net[-1].activations
        cur = 0
        for o in range(len(output)):
            if output[o] > output[cur]:
                cur = o
        return cur


