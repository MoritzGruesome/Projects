import tkinter
import time
import threading
import math
import random
import numpy as np
# import pygame

# pygame.mixer.init()
# pygame.mixer
# sfx_shoot = pygame.mixer.Sound('./laser-fire.wav')
# sfx_xplSmall = pygame.mixer.Sound('./bangSmall.wav')
# sfx_xplBig = pygame.mixer.Sound('./bangLarge.wav')
# sfx_beep1 = pygame.mixer.Sound('./beat1.wav')
# sfx_beep2 = pygame.mixer.Sound('./beat2.wav')

# sfx_shoot.set_volume(0.3)
# sfx_xplSmall.set_volume(0.3)
# sfx_xplBig.set_volume(0.3)
# sfx_beep1.set_volume(0.3)
# sfx_beep2.set_volume(0.3)
# pygame.mixer.music.set_volume(0.3)

# RL logic:
# states: [(N),(NE),(E),(SE),S,(SW),(W),(NW)]
# actions: [(turn left), (turn right), shoot]

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

state = np.zeros(10)

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

class KeyTracker:
    def __init__(self, keys=None):
        # Initialize with an optional set of keys
        self.keys = set(keys) if keys else set()
        # Track press and release times for each key in dictionaries
        self.last_press_time = {key: 0 for key in self.keys}
        self.last_release_time = {key: 0 for key in self.keys}

    def track(self, keys):
        # Update the list of keys and initialize times if new keys are added
        new_keys = set(keys) - self.keys
        self.keys.update(new_keys)
        for key in new_keys:
            self.last_press_time[key] = 0
            self.last_release_time[key] = 0

    def is_pressed(self, key):
        # Check if the given key is considered pressed
        return time.time() - self.last_press_time.get(key, 0) < 0.1 

    def report_key_press(self, event):
        # Only process if the key is in the tracked keys list
        key = event.keysym
        if key in self.keys:
            if not self.is_pressed(key):
                OnKeyPress(event)  # Trigger the press event
            self.last_press_time[key] = time.time()  # Update the press time

    def report_key_release(self, event):
        # Only process if the key is in the tracked keys list
        key = event.keysym
        if key in self.keys:
            # Start a timer to check if the key is released
            timer = threading.Timer(0.1, self.report_key_release_callback, args=[event])
            timer.start()

    def report_key_release_callback(self, event):
        # Only process if the key is in the tracked keys list
        key = event.keysym
        if key in self.keys and not self.is_pressed(key):
            OnKeyRelease(event)  # Trigger the release event
            self.last_release_time[key] = time.time()  # Update the release time

key_tracker = KeyTracker()
window.bind_all('<KeyPress>', key_tracker.report_key_press)
window.bind_all('<KeyRelease>', key_tracker.report_key_release)
key_tracker.track('w')
key_tracker.track('a')
key_tracker.track('d')
key_tracker.track("g")

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
    SPAWN_TIMES = [360, 240, 180]
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
        self.startpos = pos
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
        if self.pos[0] < -50 or self.pos[0] > cWidth+50 or self.pos[1] < -50 or self.pos[1] > cHeight + 50:
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
        # DrawLine(self.pos, a2d(self.pos, scale2d( normalize(a2d((p_x, p_y), scale2d(self.pos, -1))) , 100)), fill="red")
        # DrawLine(self.pos, a2d(self.pos, scale2d(self.dir, 50)))

asteroids = []
nextSpawn = random.randint(round(Asteroid.SPAWN_TIMES[0] / 2), Asteroid.SPAWN_TIMES[0])
lastSpawn = 0
def AstHandler():
    global asteroids, curFrame, p_x, p_y, lastSpawn, nextSpawn, p_dead, sfx_xplBig

    keep = []
    for a in range(0, len(asteroids)):
        asteroids[a].Update()
        if asteroids[a].IsPointInside((p_x, p_y)):
            sfx_xplBig.play()
            p_dead = True
        if not asteroids[a].delete:
            asteroids[a].Draw()
            keep.append(asteroids[a])
        elif asteroids[a].phase < len(Asteroid.STAGE_CORDS) - 1:
            keep.append(Asteroid(asteroids[a].pos, asteroids[a].phase + 1))
            keep.append(Asteroid(asteroids[a].pos, asteroids[a].phase + 1))
    
    asteroids = keep

    if curFrame == nextSpawn:
        left = random.randint(0,1)
        top = random.randint(0,1)
        choice = random.randint(0,1)

        pos_x = left * cWidth if choice == 0 else random.randint(0, cWidth)
        pos_y = top * cHeight if choice == 1 else random.randint(0, cHeight)

        asteroids.append( Asteroid( (pos_x, pos_y), 0, normalize( s2d( (p_x, p_y),(pos_x, pos_y) ) ) ) )
        # print((pos_x, pos_y))
        # print(math.acos(dot((0, 1), normalize(s2d( (pos_x, pos_y), (p_x, p_y) ) ))))
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
                # sfx_xplSmall.play()
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

# pygame.mixer.music.load('./thrust.wav')
# pygame.mixer.music.play(loops=-1)
# pygame.mixer.music.pause()

def PThrust():
    global wPressed
    global p_thrust, p_thrustStep, p_maxThrust
    if not wPressed: 
        p_thrust = (0,0)
        # if pygame.mixer.music.get_busy():
        #     pygame.mixer.music.pause()
        return
    # if not pygame.mixer.music.get_busy(): 
    #     pygame.mixer.music.unpause()
        
    p_thrust = a2d(p_thrust, scale2d(angleToVec(p_facing), p_thrustStep))
    tmag = mag(p_thrust)
    if (tmag > p_maxThrust):
        p_thrust = normalize(p_thrust, tmag)
        p_thrust = scale2d(p_thrust, p_maxThrust)

def PSpawnProj():
   #print("shooting")
    global p_projectiles, p_facing, p_x, p_y, sfx_shoot
    #sfx_shoot.play()
    p_projectiles.append(Projectile())
    #print("shooting")

window.bind("<Button-1>", PSpawnProj)

def PFriction():
    global wPressed, p_vel, p_friction

    spd = mag(p_vel)
    nVel = normalize(p_vel)
    if spd > 0:
        p_vel = a2d(p_vel, scale2d(nVel, -p_friction))
        if dot(nVel, p_vel) < 0:
            p_vel = (0,0)

def PRot():
    global p_facing, p_turnSpeed, aPressed, dPressed

    delta = -1 if aPressed else 1 if dPressed else 0
    p_facing += delta * p_turnSpeed

def PRote(num):
    global p_facing, p_turnSpeed, aPressed, dPressed

    
    p_facing += num * p_turnSpeed

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

def OnKeyPress(e):
    global wPressed, aPressed, dPressed, wDownFrame, curFrame, asteroids, nextSpawn, score, p_x, p_y, p_dead, p_vel
    if e.keysym == 'w':
        print("Down {}".format(curFrame))
        wDownFrame = curFrame
        wPressed = True
    if e.keysym == 'a':
        aPressed = True
    if e.keysym == 'd':
        dPressed = True
    if e.char == 'g':
        asteroids = []
        curFrame = 0
        nextSpawn = 5
        score = 0
        p_vel = (0,0)
        p_x, p_y = (round(cWidth / 2), round(cHeight / 2))
        p_dead = False

def OnKeyRelease(e):
    global wPressed, aPressed, dPressed, wDownFrame, curFrame
    if e.keysym == 'w' and curFrame - wDownFrame > 15:
        print("Up {}".format(curFrame))
        wPressed = False
    if e.keysym == 'a':
        aPressed = False
    if e.keysym == 'd':
        dPressed = False

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

def GetAstInDirect(direct):
    global cWidth, asteroids
    a = cWidth/2    # half of side length
    angle = math.radians(22.5)
    con = (a)*math.tan(angle)
    # Assuming we have a square screen of size a*a
    # The fieds which asteroids will approach in are determined by their starting position
    #states: [(E),(SE),(S),(SW),(W),(NW),(N),(NE) rotation]
    
    for ast in asteroids:
        print(ast.dir)
        if (ast.startpos[1] < a - con): # upper
            if (ast.startpos[0] > a - con):
                if (ast.startpos[0] > a + con):
                    if (direct == 7):
                        return ast #NE
                else:
                    if (direct == 6):
                        return ast #N
            else:
                if (direct == 5):
                    return ast #NW
        
        if (ast.startpos[1] > a + con): # lower
            if (ast.startpos[0] > a - con):
                if (ast.startpos[0] > a + con):
                    if (direct == 1):
                        return ast #SE
                else:
                    if (direct == 2):
                        return ast #S
            else:
                if (direct == 3):
                    return ast #SW
        
        if (ast.startpos[0] > a):
            if (direct == 0):
                return ast #E
        else:
            if (direct == 4):
                return ast #W

def getState(): # Gets the current state of the agent
    global asteroids, state
    state[:] = 0
    for ast in asteroids:
        state[AstDirect(ast)] = 1
        print(ast.dir)

    state[8] = 1*(p_facing % 360)/360
    state[9] = 1 - 1*(p_facing % 360)/360

def TurnReward(): # state[8] up 5 down 0, state[9] up 0, down 5
    global state
    m = 1
    ran = m/8
    ran2 = m - (ran/2)
    #print(ran2)
    #print(((ran2 + (1*ran))%m))

    if (state[0] > 0):
        
        if (state[8] < (ran/2) and state[8] > 0):
            print("lined up")
            PSpawnProj()
            

    if (state[0] > 0):
        if (state[8] > (ran2) and state[8] < m):
            print("lined up")
            PSpawnProj()
            
    #print(state[8])
    for i in range(1,8):
        if (state[i] > 0):
            
            if ((state[8] > ((ran2 + i*ran)%m)) and (state[8] < ((ran2+((i+1)*ran))%m))):
                print("lined up")
                PSpawnProj()

    #return 0

def GameLoop():
    global wPressed, canvas, curFrame, p_dead, state
    if p_dead:
        # if pygame.mixer.music.get_busy(): 
        #     pygame.mixer.music.pause()
        window.after(10, GameLoop)
        return
    # if curFrame % 120 == 0:
    #     sfx_beep2.play()
    # elif curFrame % 60 == 0:
    #     sfx_beep1.play()
    canvas.delete("all")
    PRot()
    PThrust()
    #PRote(22.5)
    getState()
    TurnReward()
    #print(p_facing)
    PFriction()
    PMove()
    PDraw()
    HandleProjectiles()
    AstHandler()
    DrawScore()
    curFrame += 1
    window.after(10, GameLoop)
    print(p_facing)
GameLoop()

window.mainloop()