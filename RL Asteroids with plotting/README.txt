This code was used as part of a project in Artifical intelligence and Applied Methods course at KTH.
The goal of this project was to train an agent to play the Atari game Asteroids. 
The code includes a prototype of the agent which enabled it to turn and shoot while stationary
in the middle of the screen. A copy of the final report for the project has also been uploaded to this repository as "Project Report."

TO RUN:
In order to observe the training, simply execute the code in TrainAstAgent.py.
This also saves a copy of the model parameters for the model with the current highscore

In order to test the model, execute the code in PlayByModel.py. This opens a new game environment
with an agent fully exploiting the model, based on the model stored in the folder ./model

NOTE: In order to use a pre-saved model, the file name must be inside the ./model directory and
have the name "model"
