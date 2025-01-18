import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import os

# class for creating the single hidden layer NN
class Linear_QNet(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super().__init__()
        self.linear1 = nn.Linear(input_size, hidden_size)
        self.linear2 = nn.Linear(hidden_size, output_size)

    #feed forward function
    def forward(self, x):
        x = F.relu(self.linear1(x))
        x = self.linear2(x)
        return x
    
    #function for saving the current model to file
    def save(self,file_name='model.pth'):
        model_folder_path = './model'
        if not os.path.exists(model_folder_path):
            os.makedirs(model_folder_path)
        file_name = os.path.join(model_folder_path, file_name)
        torch.save(self.state_dict(), file_name)

    def load(self, file_name='model.pth'):
        model_folder_path = './model'
        file_path = os.path.join(model_folder_path, file_name)
        if os.path.exists(file_path):
            self.load_state_dict(torch.load(file_path))
            
            print("Model loaded")
        else:
            raise FileNotFoundError(f"No such file: {file_path}")


#training function
class QTrainer:
    def __init__(self,model,lr,gamma):
        self.lr = lr # learning rate defined in agent file
        self.gamma = gamma # discount rate from agent
        self.model = model # defines the model
        #used to optimize the training? 
        #-uses Adam optimization algorithm
        #-This will improve the efficiency of model updates
        #- The algorithm keeps track of the mean and variance of gradients over time
        #- its updates the parameters using a combination of these values
        self.optimizer = optim.Adam(model.parameters(),lr=self.lr)
        #gets the loss function (Mean root squared error)
        #self.criterion = nn.MSELoss()
        #Ended up using the SmoothL1
        self.criterion = nn.SmoothL1Loss()

    def train_step(self,state,action,reward,next_state,done):
        #convert arguments to tensors
        state = torch.tensor(state,dtype=torch.float)
        next_state = torch.tensor(next_state,dtype=torch.float)
        action = torch.tensor(action,dtype=torch.long)
        reward = torch.tensor(reward,dtype=torch.float)
        # size(n,x)

        #only 1 dimension
        if len(state.shape) == 1:
            # (1,x) changes the dimensions of the tensors
            state = torch.unsqueeze(state,0)
            next_state = torch.unsqueeze(next_state,0)
            action = torch.unsqueeze(action,0)
            reward = torch.unsqueeze(reward,0)
            done = (done, )
        
        #1: predicted Q values with the current state
        pred = self.model(state) # this is Q(s,a)

        # Now we calculate the target Q values for a batch of experiences
        # The tensors defined above, state for example contains N items of state, where N is the batch size
        # We calculate the target Q values for all of these experience in one go, for one batch

        target = pred.clone() # we make a clone of Q(s,a)
        # since the length of the done tensor is equal to all others(state,next_state,action etc)
        # we can iterate over this range for all experiences in a batch
        for idx in range(len(done)): 
            # simple case for when done, since there is no next state
            # (in this case the reward will include a penatly for dying)
            Q_new = reward[idx]
            # for the cases when we are still playing however, we must account for future rewards
            # these are accounted for by Q(s',a') -> torch.max(self.model(next_state[idx]))
            if not done[idx]:
                Q_new = reward[idx] + self.gamma * torch.max(self.model(next_state[idx]))
            
            # target is a copy of the predicted Q values for the current state
            # action[idx] is the action taken in the current state. 
            # The torch.argmax(action[idx]) retrieves the index of the chosen action (e.g., 0, 1, or 2).
            # The target Q-value for this action is updated to Qnew
            target[idx][torch.argmax(action[idx]).item()] = Q_new

        # resets the gradients of all the optimized torch.tensors
        self.optimizer.zero_grad()
        # Calculates the Mean root squared error using the target
        # and the pred values. 
        # note: since we are using minibatches ...  explain how backprop is done concurrently with minibatches
        loss = self.criterion(target,pred)
        # Here the gradients of the loss wih respect to the model parameters is calculated
        loss.backward()
        # finally we adjust the models parameters using these gradients and the Adam Algorithm

        self.optimizer.step()

        

        