import tkinter
import torch
from collections import deque
from model import Linear_QNet,QTrainer
import math
import random
import numpy as np
import time


n_games = 0 # number of games played

model = Linear_QNet(10,256,3) 
model.load('model')
plot_scores = [] # list of scores for games
plot_mean_scores = [] # mean score for games
total_score = 0 # total score for current game
record = 0 # best score accross all games
average = 0

# global variables for game
random.seed()
deg2Rad = 0.01745329251

window = tkinter.Tk()
window.title("Asteroids")

cWidth, cHeight = (800, 800)
canvas = tkinter.Canvas(window, width=cWidth, height=cHeight, bg="black")
canvas.pack()
curFrame = 0
wPressed = aPressed = dPressed = False
wDownFrame = 0

#[(E),(SE),(S),(SW),(W),(NW),(N),(NE) rotation clockwise, rotation anticlockwise]
compassDeg = np.array([1,0.125,0.25,0.375,0.5,0.625,0.75,0.875])


state = np.zeros(10) # the states of the game
state_old = np.zeros(10) # keeps previous state of the game
action = np.zeros(3) # keeps the current action of the agent


# extra math functions
def sgn(num):
    if (num < 0):
        return -1
    else:
        return 1

def normalize(vec2d, magnitude=None):
    x,y=vec2d
    if magnitude == None:
        magnitude = mag(vec2d)
    if magnitude < 0.01:
        return (0,0)
    return (x / magnitude, y / magnitude)

def mag(vec2d):
    (x,y) = vec2d
    return math.sqrt(x*x + y*y)

def sqmag(vec2d):
    (x,y) = vec2d
    return x*x + y*y
# add
def a2d(v1, v2):
    (v1x, v1y) = v1
    v2x, v2y = v2
    return (v1x + v2x, v1y + v2y)
# subtract
def s2d(v1, v2):
    return a2d(v1, scale2d(v2, -1))

def scale2d(v1, a):
    v1x, v1y = v1
    return (v1x * a, v1y * a)

def dot(v1, v2):
    x1,y1 = v1
    x2,y2 = v2
    return x1*x2 + y1*y2

# a in degrees
def angleToVec(a):
    global deg2Rad
    return (math.cos(a * deg2Rad), math.sin(a * deg2Rad))
    

def DrawPixel(x,y, outline="white", fill="white"):
    canvas.create_rectangle(x, y, x + 1, y + 1, outline=outline, fill=fill)

def DrawLine(start, end, outline="white", fill="white"):
    (cx, cy) = start
    cx = round(cx)
    cy = round(cy)
    (tx, ty) = end
    tx = round(tx)
    ty = round(ty)

    canvas.create_line(cx,cy,tx,ty, fill=fill)


class ParticleEffect():
    ITERS = 60
    def __init__(self, pos):
        self.points = []
        self.pos = pos
        self.iteration = 0

        for _ in range(0, 5):
            d=normalize( (random.uniform(-1,1), random.uniform(-1,1)) )
            self.points.append(( pos[0], pos[1], d[0], d[1] ))

    def play(self):
        global window, p_dead
        for i in range(0, len(self.points)):
            #print(self.points[i][0])
            nx, ny = a2d((self.points[i][0], self.points[i][1]), (self.points[i][2], self.points[i][3]))
            self.points[i] = (nx,ny,self.points[i][2],self.points[i][3])
            DrawPixel(self.points[i][0], self.points[i][1])
        self.iteration += 1
        if self.iteration >= ParticleEffect.ITERS or p_dead:
            return
        window.after(round(1/60 * 1000), self.play)

class Asteroid:
    SPAWN_TIMES = [720, 720, 720]
    MIN_SPEED = 2
    MAX_SPEED = 2
    ROT_SPEED = 1
    STAGE_CORDS = [
        # [(32,32), (32,-32), (-32,-32), (-32,32)]
        [(16,64), (16,32), (32,16), (64,16), (32,-48), (-48,-40), (-64,32), (-48,48), (-32,16), (-16,64), (16,64)],
        [(4,12), (8,4), (16,0), (8,-12), (4,-20), (-8,-20), (-16,-12), (-20, -4), (-8, 4), (-4, 12)]
        # [(4,16), (4,8), (8,16), (16,4), (8,-12), (-12,-10), (-16,8), (-12,12), (-8,4), (-4,16), (4,16)]
    ]
    def __init__(self, pos, phase=0, direction=None):
        self.startpos = pos # stores start position of asteroid
        self.pos = pos
        self.phase = phase
        if direction == None:
            a = random.randint(0, 359)
            direction = (math.cos(a * deg2Rad), math.sin(a * deg2Rad))
        self.dir = direction
        self.delete = False
        self.speed = random.uniform(Asteroid.MIN_SPEED, Asteroid.MAX_SPEED)
        self.rotAngle = 0

    def Update(self):
        global p_x, p_y
        self.rotAngle += Asteroid.ROT_SPEED
        self.pos = a2d(self.pos, scale2d(self.dir, self.speed))
        # on this line 30 was 50
        if self.pos[0] < -30 or self.pos[0] > cWidth+30 or self.pos[1] < -30 or self.pos[1] > cHeight + 30:
            self.delete = True

    def IsPointInside(self, point):
        maxX = 0
        minX = 0
        maxY = 0
        minY = 0
        for c in Asteroid.STAGE_CORDS[self.phase]:
            if maxX < c[0]:
                maxX = c[0]
            elif minX > c[0]:
                minX = c[0]
            if maxY < c[1]:
                maxY = c[1]
            elif minY > c[1]:
                minY = c[1]
        
        return point[0] < maxX + self.pos[0] and point[0] > minX + self.pos[0] and point[1] < maxY + self.pos[1] and point[1] > minY + self.pos[1]
    
    def Draw(self):
        global canvas, p_x, p_y
        pointList = Asteroid.STAGE_CORDS[self.phase].copy()

        for i in range(0, len(pointList)):
            m = mag(pointList[i])
            dotp = dot((1,0), normalize(pointList[i]))
            dotp = 1 if dotp > 1 else -1 if dotp < -1 else dotp
            a = math.acos(dotp)
            
            if dot((0,1), pointList[i]) < 0:
                a *= -1
            pointList[i] = a2d(self.pos, (m * math.cos(self.rotAngle * deg2Rad + a), m * math.sin(self.rotAngle * deg2Rad + a)) )

        canvas.create_polygon(pointList, outline="white", fill="")
        

asteroids = [] # list of current asteroids
nextSpawn = random.randint(round(Asteroid.SPAWN_TIMES[0] / 2), Asteroid.SPAWN_TIMES[0])
lastSpawn = 0
def AstHandler():
    global asteroids, curFrame, p_x, p_y, lastSpawn, nextSpawn, p_dead

    keep = []
    for a in range(0, len(asteroids)):
        asteroids[a].Update()
        if asteroids[a].IsPointInside((p_x, p_y)):
            p_dead = True
        if not asteroids[a].delete:
            asteroids[a].Draw()
            keep.append(asteroids[a])
    
    asteroids = keep

    if curFrame == nextSpawn:
        left = random.randint(0,1)
        top = random.randint(0,1)
        choice = random.randint(0,1)

        pos_x = left * cWidth if choice == 0 else random.randint(0, cWidth)
        pos_y = top * cHeight if choice == 1 else random.randint(0, cHeight)

        asteroids.append( Asteroid( (pos_x, pos_y), 0, normalize( s2d( (p_x, p_y),(pos_x, pos_y) ) ) ) )
        lastSpawn = curFrame
        spnOffset = Asteroid.SPAWN_TIMES[0]

        if curFrame > 3600: 
            spnOffset = Asteroid.SPAWN_TIMES[1]
        elif curFrame > 7200:
            spnOffset = Asteroid.SPAWN_TIMES[2]

        nextSpawn = curFrame + random.randint(round(spnOffset/2) if curFrame < 1200 or len(asteroids) > 10 else 10, spnOffset)
(p_x, p_y) = (round(cWidth / 2), round(cHeight / 2))

p_facing = 0 # clockwise angle from east (degrees)
p_turnSpeed = 4
p_legLength = 40

p_thrust = (0,0)
p_thrustStep = 0.005
p_maxThrust = .035
p_vel = (0,0)
p_maxSpeed = 2.5
p_friction = .01

p_projectiles = []
p_projSpeed = 20

p_dead = False

score = 0
def Score(s):
    global score
    score += s

def DrawScore():
    global score
    canvas.create_text( round(cWidth / 2), 100, justify='center',  fill="white", text="SCORE: {}".format(score), font=("Courier", 36), )

# Projectile information: x,y coordinates, dirVec: (dx,dy)
class Projectile:
    def __init__(self):
        global p_x, p_y, p_facing
        self.x = p_x
        self.y = p_y
        self.dirVec = angleToVec(p_facing)

    def __eq__(self, other):
        if self is None and other is None:
            return True
        if other is None:
            return False
        return self.x == other.x and self.y == other.y and self.dirVec == other.dirVec


def HandleProjectiles():
    global p_projectiles, p_projSpeed, asteroids
    keep = []
    for projectile in p_projectiles:
        cx,cy = (projectile.x, projectile.y)
        dx,dy = projectile.dirVec
        nx,ny = (cx + dx * p_projSpeed, cy + dy * p_projSpeed)
        if nx < 0 or nx > cWidth or ny < 0 or ny > cHeight:
            continue
        delProj = False
        for asteroid in asteroids:
            if (not asteroid.delete) and asteroid.IsPointInside((projectile.x, projectile.y)):
                Score((len(Asteroid.STAGE_CORDS) - asteroid.phase) * 10)
                asteroid.delete = True
                ps = ParticleEffect(asteroid.pos)
                ps.play()
                delProj = True
                break
        if delProj:
            continue
        projectile.x = nx
        projectile.y = ny
        keep.append(projectile)

        DrawPixel(nx,ny)
    p_projectiles = keep

def PDraw():
    global p_x, p_y, p_legLength, deg2Rad, p_thrust
    pLegAngle = 165
    dx1 = p_legLength * math.cos((p_facing + pLegAngle) * deg2Rad)
    dx2 = p_legLength * math.cos((p_facing - pLegAngle) * deg2Rad)
    dy1 = p_legLength * math.sin((p_facing + pLegAngle) * deg2Rad)
    dy2 = p_legLength * math.sin((p_facing - pLegAngle) * deg2Rad)
    
    b1 = (p_x+dx1*0.8, p_y+dy1*0.8)
    b2 = (p_x+dx2*0.8, p_y+dy2*0.8)

    baseMidPoint = ((b2[0] - b1[0]) / 2, (b2[1] - b1[1]) / 2)
    t1 = a2d(b1,scale2d(baseMidPoint, 0.3))
    t2 = a2d(b1,scale2d(baseMidPoint, 1.7))
    t3 = scale2d((p_legLength * 1.3 * math.cos(p_facing * deg2Rad), p_legLength * 1.3 * math.sin(p_facing * deg2Rad)), -1)

    DrawLine(b1, b2)
    DrawLine((p_x, p_y), (p_x + dx1, p_y + dy1))
    DrawLine((p_x, p_y), (p_x + dx2, p_y + dy2))
    if mag(p_thrust) > 0.01:
        DrawLine(t1, (p_x + t3[0], p_y + t3[1]))
        DrawLine(t2, (p_x + t3[0], p_y + t3[1]))



def PThrust():
    global wPressed
    global p_thrust, p_thrustStep, p_maxThrust
    if not wPressed: 
        p_thrust = (0,0)
        return
    
        
    p_thrust = a2d(p_thrust, scale2d(angleToVec(p_facing), p_thrustStep))
    tmag = mag(p_thrust)
    if (tmag > p_maxThrust):
        p_thrust = normalize(p_thrust, tmag)
        p_thrust = scale2d(p_thrust, p_maxThrust)

def PSpawnProj():
    #print("shooting")
    global p_projectiles, p_facing, p_x, p_y
    p_projectiles.append(Projectile())
    #print("shooting")

def PFriction():
    global wPressed, p_vel, p_friction

    spd = mag(p_vel)
    nVel = normalize(p_vel)
    if spd > 0:
        p_vel = a2d(p_vel, scale2d(nVel, -p_friction))
        if dot(nVel, p_vel) < 0:
            p_vel = (0,0)

def AstDist(asteroid):
    global p_x, p_y, cWidth, cHeight
    d = mag(s2d((p_x,p_y),asteroid.pos))
    d = 1 - ((d / (mag(s2d((p_x,p_y),asteroid.startpos)))))
    return d

def AstDirect(asteroid):
    global cWidth
    ast = asteroid
    a = cWidth/2    # half of side length
    angle = math.radians(22.5)
    con = (a)*math.tan(angle)
    # Assuming we have a square screen of size a*a
    # The fieds which asteroids will approach in are determined by their starting position
    #states: [(E),(SE),(S),(SW),(W),(NW),(N),(NE) rotation]
    if (ast.startpos[1] < a - con): # upper
        if (ast.startpos[0] > a - con):
            if (ast.startpos[0] > a + con):
                return 7 #NE
            else:
                return 6 #N
        else:
            return 5 #NW
    
    if (ast.startpos[1] > a + con): # lower
        if (ast.startpos[0] > a - con):
            if (ast.startpos[0] > a + con):
                return 1 #SE
            else:
                return 2 #S
        else:
            return 3 #SW
    
    if (ast.startpos[0] > a):
        return 0 #E
    else:
        return 4 #W
    
    

#actions = [x1,x2,x3] xi is 1 or 0, actions denotes [(turn left), (turn right), (shoot)]
def AgentAction(actions):
    global p_facing, p_turnSpeed

    delta = -1 if (actions[0] == 1) else 1 if (actions[1] == 1) else 0
    p_facing += delta * p_turnSpeed

    if (actions[2] == 1):
        PSpawnProj()


def PMove():
    global p_facing, p_x, p_y, deg2Rad, p_thrust, p_vel, p_maxSpeed

    p_vel = a2d(p_thrust, p_vel)
    spd = mag(p_vel)
    if spd > p_maxSpeed:
        nVel = normalize(p_vel, spd)
        p_vel = scale2d(nVel, p_maxSpeed)

    p_x += p_vel[0]
    p_y += p_vel[1]

    if p_x < 10:
        p_x = 10
    if p_y < 10:
        p_y = 10
    if p_y > cHeight - 10:
        p_y = cHeight - 10
    if p_x > cWidth - 10:
        p_x = cWidth - 10

def ResetGame():
    global asteroids, curFrame, nextSpawn, score, p_vel, p_x, p_y, p_dead,p_facing
    asteroids = []
    curFrame = 0
    nextSpawn = 5
    score = 0
    p_vel = (0,0)
    p_x, p_y = (round(cWidth / 2), round(cHeight / 2))
    p_facing = random.uniform(0, 360)
    p_dead = False

####### RL functions ###############################################

def getState(): # Gets the current state of the agent
    global asteroids, state
    state[:] = 0
    for ast in asteroids:
        state[AstDirect(ast)] = AstDist(ast)

    state[8] = (p_facing % 360)/360
    state[9] = 1 - ((p_facing % 360)/360)
    
# returns an action which will be taken by agent
# format for action is [1,0,0], only one element is 1, rest are 0's
def get_action(state):
    global model
    
    final_move = [0,0,0]

    #use model to determine next move
    #current state, convert to tensor
    state0 = torch.tensor(state,dtype=torch.float)
    #input tensor into model
    prediction = model(state0)
    #next move is the maximum value of output layer
    move = torch.argmax(prediction).item()
    final_move[move] = 1

    return final_move
####################################################################

# play step should be called every X frames, we can start by testing every 10 frames
# return reward, game_over, self.score / after each play_step
def Agentmove():
    global action, state
    action = get_action(state)
    AgentAction(action) 

    
def playstep():

    global wPressed, canvas, curFrame, p_dead, n_games, score, record, action, state, total_score, average
    if p_dead:
        
        if score > record:
            record = score
        
        total_score += score
        if (n_games > 0):
            average = total_score/n_games

        print('Game', n_games, 'score', score, 'Record:', record, 'average:', average)
        
        ResetGame()
        n_games += 1
        
    if (curFrame % 2 == 0): # every 10 frames
        Agentmove()

    canvas.delete("all")
    
    PFriction()
    PMove()
    PDraw()
    HandleProjectiles()
    AstHandler()
    DrawScore()
    getState()
        
    curFrame += 1
    
    
    window.after(3, playstep)

    if (n_games > 1000):
        exit()
    

playstep()
window.mainloop()

