import game_core
import pygame
from pygame.locals import *
import sys

#Control values
SCREEN_WIDTH = 1000
SCREEN_HEIGHT = 1000

WORLD_WIDTH = 200
WORLD_HEIGHT = 200

CELL_PIXEL_SIZE = 20

RANDOM_START = False

GENERATION_RATE = 10
FPS = 60


pygame.init()

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Game of life')

frames = pygame.time.Clock()

font = pygame.font.SysFont("Arial" , 18 , bold = True)

paused = False
lpressed = False
rpressed = False
mpressed = False
last_mposition = pygame.mouse.get_pos()
offset = [WORLD_WIDTH/2*CELL_PIXEL_SIZE, WORLD_HEIGHT/2*CELL_PIXEL_SIZE]
offset_max = [WORLD_WIDTH*CELL_PIXEL_SIZE-SCREEN_WIDTH, WORLD_HEIGHT*CELL_PIXEL_SIZE-SCREEN_HEIGHT]
p_pressed = False


sX = 0
sY = 0
x = 0
y = 0

counter = 0

def calc():
    global sX, sY, x, y
    sX = int(offset[0]/CELL_PIXEL_SIZE)
    sY = int(offset[1]/CELL_PIXEL_SIZE)

    x = -offset[0] % CELL_PIXEL_SIZE
    y = -offset[1] % CELL_PIXEL_SIZE
    
    #print(offset[0], offset[1])

def fps_counter():
    fps = str(int(frames.get_fps()))
    fps_t = font.render(fps , 1, pygame.Color("WHITE"))
    screen.blit(fps_t,(0,0))
  
def update():
    screen.fill((30, 30, 30))
    
    for i in range(int(SCREEN_WIDTH/CELL_PIXEL_SIZE)):
        pygame.draw.line(screen, (127, 127, 120), (x+i*CELL_PIXEL_SIZE, 0), (x+i*CELL_PIXEL_SIZE, SCREEN_HEIGHT))
    for i in range(int(SCREEN_HEIGHT/CELL_PIXEL_SIZE)):
        pygame.draw.line(screen, (127, 127, 120), (0, y+i*CELL_PIXEL_SIZE), (SCREEN_WIDTH, y+i*CELL_PIXEL_SIZE))
    
    for i in range(int(SCREEN_HEIGHT/CELL_PIXEL_SIZE)):
        for j in range(int(SCREEN_WIDTH/CELL_PIXEL_SIZE)):
            if game_core.world[sX+j][sY+i] == 1:
                pygame.draw.rect(screen, (204, 204, 193), (x+j*CELL_PIXEL_SIZE, y+i*CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE))

game_core.initialize((WORLD_WIDTH, WORLD_HEIGHT))
calc()

while True:
    pygame.display.update()
    mouse = pygame.mouse.get_pos() 
    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_ESCAPE]:
        pygame.quit()
        sys.exit()
    if pressed_keys[K_p]:
        if not p_pressed:
            if paused: paused = False
            else: paused = True
            p_pressed = True
    elif p_pressed:
        p_pressed = False
    
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN:
            p = pygame.mouse.get_pressed(num_buttons=3)
            if p[0]:
                lpressed = True
            if p[1]:
                mpressed = True
            if p[2]:
                rpressed = True
        if event.type == pygame.MOUSEBUTTONUP:
            p = pygame.mouse.get_pressed(num_buttons=3)
            if not p[0]:
                lpressed = False
            if not p[1]:
                mpressed = False
            if not p[2]:
                rpressed = False
    
    if lpressed:
        sqx = int((mouse[0]+x)/CELL_PIXEL_SIZE)
        sqy = int((mouse[1]+y)/CELL_PIXEL_SIZE)
        game_core.world[sX+sqx][sY+sqy] = 1    
    
    if rpressed:
        sqx = int((mouse[0]+x)/CELL_PIXEL_SIZE)
        sqy = int((mouse[1]+y)/CELL_PIXEL_SIZE)
        game_core.world[sX+sqx][sY+sqy] = 0 
        
    if mpressed:
        if mouse != last_mposition:
            offset = [offset[0] - mouse[0] + last_mposition[0], offset[1] - mouse[1] + last_mposition[1]]
            if offset[0] < 0 : offset[0] = 0
            elif offset[0] > offset_max[0] : offset[0] = offset_max[0]
            if offset[1] < 0 : offset[1] = 0
            elif offset[1] > offset_max[1] : offset[1] = offset_max[1]
            
            calc()
    last_mposition = mouse
        
    if not paused:
        counter += 1
        if counter == int(FPS/GENERATION_RATE):
            game_core.step()
            counter = 0
        
    update()
    fps_counter()

    frames.tick(FPS)
