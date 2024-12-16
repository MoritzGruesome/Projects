import numpy as np
import networkFrame as frame
import ForewardProp as fp
import MNIST as mnist

class backprop:

    #init functin for backprop class
    #takes an array training data from the MNIST package
    #takes a network from network class
    def __init__(self,trainingData,network):
      
      self.checkMatrix = np.array([[[1],[0],[0],[0],[0],[0],[0],[0],[0],[0]],
                            [[0],[1],[0],[0],[0],[0],[0],[0],[0],[0]],
                            [[0],[0],[1],[0],[0],[0],[0],[0],[0],[0]],
                            [[0],[0],[0],[1],[0],[0],[0],[0],[0],[0]],
                            [[0],[0],[0],[0],[1],[0],[0],[0],[0],[0]],
                            [[0],[0],[0],[0],[0],[1],[0],[0],[0],[0]],
                            [[0],[0],[0],[0],[0],[0],[1],[0],[0],[0]],
                            [[0],[0],[0],[0],[0],[0],[0],[1],[0],[0]],
                            [[0],[0],[0],[0],[0],[0],[0],[0],[1],[0]],
                            [[0],[0],[0],[0],[0],[0],[0],[0],[0],[1]]])
      
      self.trainingData = trainingData
      self.network = network

    #function returns the derivative of the sigmoid function
    def sigmoid_derivative(self,x):
        sig = fp.ForewardProp.sigmoid(x)
        return sig * (1 - sig)
    
    #calculates error vector of the last layer in network for one training example
    #takes argument number referring to the correct label of the digit, this is an integer
    def OutputError(self,number):
       error = self.network.net[-1].activations - self.checkMatrix[number]
       error = error * self.sigmoid_derivative(self.network.net[-1].sum)
       return (error.reshape(-1,1))
    
    #calculates the error of the (l)th from the (l+1)th layer argument
    #takes layer which is an integer referring to the layer index
    def BackPropError(self,layer):
        
        errorsCol = self.network.net[layer+1].errors
        weightsT = self.network.net[layer+1].weights.T
        
        error = np.matmul(weightsT,errorsCol)
        sigDerSum = self.sigmoid_derivative(self.network.net[layer].sum)
        error = error * sigDerSum
        return error
    
    #Finds the errors for all layers in the network
    #for each adjacent layer we propagate the error to the left until we reach the input layer
    #takes argument number which is the correct label of the digit
    def Backpropagation(self,number):
       self.network.net[-1].errors = self.OutputError(number)
       netSize = len(self.network.net)
       for l in range(1,netSize-1):
          self.network.net[netSize-l-1].errors = self.BackPropError(netSize-l-1)
   
    
    
    #Implementation of simple gradient descent algorithm
    def GD(self,propagator,learnRate):
       
       t = self.trainingData.Database
       for i in range(60000):
          #propagate current training example through network
          self.network.net = propagator.forwardprop(self.network.net,t[i].trainingData)
          #calculate error based on output activations
          #then backpropagate the error
          self.Backpropagation(t[i].label)
          #update weights and biases based on errors
          self.Descend(learnRate)
   
   #Implementation of Stochastic gradient descent algorithm 
    def SGD(self,propagator,learnRate,BatchSize):
       
       # Generate a numpy array of length n with unique integers in the range 0 to m-1
       # used to shuffle data before training
       indexes = np.random.choice(60000, 60000, replace=False)
       batch = 0
       #iterates through entire mnist training dataset
       for t in indexes:
         
         #propagate current training example through network
         self.network.net = propagator.forwardprop(self.network.net,self.trainingData.Database[t].trainingData)
         #calculates error based on output activations, then backpropagates the error through all layers except first
         self.Backpropagation(self.trainingData.Database[t].label)
         
         #for all layers in network update sum of mini batch errors
         #completes the sum of the gradients of the cost function for each mini batch
         for l in range(1,len(self.network.net)):
          errors = self.network.net[l].errors
          PrevActivations = self.network.net[l-1].activations
          self.network.net[l].weightErrors = self.network.net[l].weightErrors + np.matmul(errors,PrevActivations.T)
          self.network.net[l].biasErrors = self.network.net[l].biasErrors + errors
          
         batch = batch + 1
         
         #Weights and biases are updated after each batch
         if batch % BatchSize == 0:
            self.DescendSGD(learnRate,BatchSize)
           
    
    #takes argument u which is the learning rate
    #The greater the size of u, the larger the steps during gradient descent
    def Descend(self,u):
       for l in range(1,len(self.network.net)):
          errors = self.network.net[l].errors
          PrevActivations = self.network.net[l-1].activations
          self.network.net[l].weights = self.network.net[l].weights - (u * np.matmul(errors,PrevActivations.T))
          self.network.net[l].bias = self.network.net[l].bias - (u * errors)
   
   #u is the learning rate
   #m is the size of a mini batch
    def DescendSGD(self, u, m):
       for l in range(1,len(self.network.net)):
          Werrors = self.network.net[l].weightErrors
          Berrors = self.network.net[l].biasErrors
          self.network.net[l].weights = self.network.net[l].weights - ((u / m) *  Werrors)
          self.network.net[l].bias = self.network.net[l].bias - ((u / m) * Berrors)
          self.network.net[l].weightErrors = self.network.net[l].weightErrors * 0
          self.network.net[l].biasErrors = self.network.net[l].biasErrors * 0
          
    #used for testing the network      
    def testNetwork(self,propagator,nist):
       score = 0
       for e in range(10000):
         if propagator.getPredictedNum(self.network.net,e,nist):
            score += 1
  
       print('final score: ' + str(score) + '/10000')
       return score    

    
    
    
    


   


 







    
    