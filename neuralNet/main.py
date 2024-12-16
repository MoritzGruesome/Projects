import BackPropagation as bp
import networkFrame as frame
import MNIST as mnist
import ForewardProp as fp
import DrawingBoard as board




class GetNet:

    def __init__(self,network,learningRate,Batchsize,Epochs):
        #creates network
        self.network = frame.net(network)
        #creates object for foreward propagation
        self.propagator = fp.ForewardProp()
        #creates object for training and testing data sets
        nist = mnist.mnist()
        #creates object for training through backpropagation
        backproper = bp.backprop(nist,self.network)

        for e in range(Epochs):
   
            backproper.SGD(self.propagator,learningRate,Batchsize)
            newscore = backproper.testNetwork(self.propagator,nist)
      
            print('epoch: ' + str(e) + 'score: ' + str(newscore) + '/10000')
        
        self.network = backproper.network
    
    




net = GetNet([784,30,10],3,10,10)

for x in range(50):
    drawer = board.graph(600)
    result = net.propagator.PredictNumFromGrid(net.network.net,drawer.normalGrid)
    print(str(result))







