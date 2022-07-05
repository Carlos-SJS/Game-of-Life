import numpy as np

world = np.ndarray((1,1))
w_size = (1,1)

def initialize(world_size):
    global world, w_size
    world = np.ndarray(world_size)
    w_size = world_size
    
def step():
    tmp = world.copy()
    for i in range(w_size[0]):
        for j in range(w_size[1]):
            s = 0
            c1 = i > 0 
            c2 = i < w_size[0]-1
            c3 = j > 0
            c4 = j < w_size[1]-1
            if c1: s = tmp[i-1][j]
            if c2: s += tmp[i+1][j]
            if c3: s += tmp[i][j-1]
            if c4: s += tmp[i][j+1]
            if c1 and c3: s += tmp[i-1][j-1]
            if c1 and c4: s += tmp[i-1][j+1]
            if c2 and c3: s += tmp[i+1][j-1]
            if c2 and c4: s += tmp[i+1][j+1]
                
            if s == 3: world[i][j] = 1
            elif world[i][j] == 1 and s == 2: world[i][j] = 1
            else: world[i][j] = 0
