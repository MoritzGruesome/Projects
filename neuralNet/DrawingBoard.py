import pygame
import numpy as np
import matplotlib.pyplot as plt

class graph:

    #takes side length of drawing board window
    #this will scale the number of boxes according to sidelength
    def __init__(self, sidelength):
        pygame.init()
        self.grid = np.zeros((28, 28))
        self.normalGrid = self.exampleExtract(self.grid) / 255
        self.window = pygame.display.set_mode((sidelength,sidelength))
        self.boxsize = sidelength // 28
        self.window.fill((255,255,255))
        #Rect(topleft, top, width, height)
        self.drawGrid()
        pygame.display.update()
        #start drawing function
        self.run()
        

    #resets all of the boxes to (0,0,0)
    def drawGrid(self):
        for c in range(28):
            for r in range(28):
                pygame.draw.rect(self.window, (0,0,0),[self.boxsize*c,self.boxsize*r,self.boxsize,self.boxsize], 1)
    
    #gets position of mouse and calculates boxes to fill
    def on_click(self,pos):
        
        x,y = pos
        #from coordinate to box coordinate
        x = x // self.boxsize
        y = y // self.boxsize
        self.grid[y][x] = 255
        self.grid[y+1][x] = 255
        self.grid[y][x-1] = 255
        self.normalGrid = self.exampleExtract(self.grid) / 255
        #print(str(x) + ',' + str(y) + ' set to filled')
        pygame.draw.rect(self.window, (0,0,0),[self.boxsize*x,self.boxsize*y,self.boxsize,self.boxsize])
        pygame.draw.rect(self.window, (150,150,150),[self.boxsize*x,self.boxsize*(y+1),self.boxsize,self.boxsize])
        pygame.draw.rect(self.window, (150,150,150),[self.boxsize*(x-1),self.boxsize*y,self.boxsize,self.boxsize])
        


    def run(self):
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                mouse_buttons = pygame.mouse.get_pressed()
                if mouse_buttons[0]:
                    self.on_click(pygame.mouse.get_pos())
                    pygame.display.update()
                if mouse_buttons[2]:
                    self.window.fill((255,255,255))
                    self.drawGrid()
                    self.grid = np.zeros((28, 28))
                    self.normalGrid = self.exampleExtract(self.grid) / 255
                    pygame.display.update()
                    
        pygame.quit()
    
    #function for displaying the drawn number
    def displayNum(self):      
        plt.imshow(self.grid, cmap='gray')
        plt.colorbar()
        plt.show()
    
    def exampleExtract(self,image):
         list = []
         for l in image:
             for pix in l:
                list.append(pix)
         return np.array(list)
    

#test = graph(600)
#test.displayNum()
