

import numpy as np
import matplotlib.pyplot as plt



class entry:

    def __init__(self,image,label,data,trainingData):
        self.image = image
        self.label = label
        self.data = data
        self.trainingData = trainingData


class mnist:

    def __init__(self):

        TestSetImageFile = open("/Users/moritzgruss/Desktop/Neural net/TrainingData/t10k-images-idx3-ubyte","rb")
        TestSetLabels = open("/Users/moritzgruss/Desktop/Neural net/TrainingData/t10k-labels-idx1-ubyte","rb")

        TrainingSetImageFile = open("/Users/moritzgruss/Desktop/Neural net/TrainingData/train-images-idx3-ubyte","rb")
        TrainingSetLabels = open("/Users/moritzgruss/Desktop/Neural net/TrainingData/train-labels-idx1-ubyte","rb")
        
        #Database is the main array storing 10000 triples of data,label and image
        #data contains greyscale encodings in 1 dimensional array
        #label contains an integer denoting label
        #image contains 2 dimensional array for displaying image in numpy
        #the pairs are stored as 1 dimensional arrays of length 3
        self.Database = []
        self.TestData = []

        for n in range(60000):
            image = self.getPixelMatrix(n,TrainingSetImageFile)
            label = self.getLabel(n,TrainingSetLabels)
            data = self.ExampleExtract(image)
            trainingData = data / 255
            entr = entry(image,label,data,trainingData)

            self.Database.append(entr)
        self.Database = np.array(self.Database)
        
        for n in range(10000):
            image = self.getPixelMatrix(n,TestSetImageFile)
            label = self.getLabel(n,TestSetLabels)
            data = self.ExampleExtract(image)
            testdata = data / 255
            entr = entry(image,label,data,testdata)

            self.TestData.append(entr)
        self.Testbase = np.array(self.TestData)
    
    #takes a training example which is stored as an array of 784 pixel values ranging between 0-255
    #returns an array where each pixel value is squashed into a decimal between 0 and 1
    def squashPix(self,example):
        newpixels = []
        for p in example:
            newpixels.append(p/255)
        return np.array(newpixels)


    #we have 28 rows and 28 columns, 28 * 28 = 784
    #function returns pixelmatrix for n'th digit in set
    def getPixelMatrix(self,n,set):
        offset = 16 + 784 * n  # Calculate the offset for the image
        set.seek(offset)  # Reset file pointer to the beginning of the image data
        data = set.read(784)  # Read 784 bytes for one image
        vector = []
        z = 0
        for x in range(28):
            row = []
            for y in range(28):
                row.append(data[z])
                z = z+1
            vector.append(row)
        return np.array(vector)

    #function extracts the label of the image
    def getLabel(self,n,set):
        offset = 8+n
        set.seek(offset)
        data = set.read(1)
        return int.from_bytes(data, byteorder='big')

    #takes index of training example from MNIST and displays
    #while also printing label
    def outputTrainingExam(self,n):      
        #PixelMatrix = self.getPixelMatrix(n,self.TrainingSetImageFile)
        PixelMatrix = self.Database[n].image
        print(self.Database[n].label)
        plt.imshow(PixelMatrix, cmap='gray')
        plt.colorbar()
        plt.show()
    
    #takes an index number example
    #extracts the greyscale pixel data from the 2 dimensional image data in the database at that index
    #returns the data reduced into a 1 dimensional array
    def ExampleExtract(self,image):
         list = []
         for l in image:
             for pix in l:
                list.append(pix)
         return np.array(list)
        

#test = mnist()
#test.outputTrainingExam(10)
#test.outputTrainingExam(4)
#print(test.Database[5][1])




