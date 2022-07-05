#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

int SCREEN_HEIGHT = 1000;
int SCREEN_WIDTH = 1600;

int WORLD_HEIGHT = 2500;
int WORLD_WIDTH = 2500;
int CELL_PXEL_SIZE = 10;

vector<vector<bool>> board(WORLD_WIDTH);
vector<vector<bool>> tmp;
queue<pair<int, int>> q;
unordered_set<string> added;

string hashP(pair<int, int> p ){
    std::ostringstream sstr;
    ( sstr << p.first << "," << p.second );
    return sstr.str();
}

void gof_step(pair<int, int> cds){
    if(added.find(hashP(cds)) != added.end()) return;
    else added.insert(hashP(cds));
    int x = cds.first, y = cds.second;
    if(x<0 || y<0 || x >= WORLD_WIDTH || y >= WORLD_HEIGHT) return;
    bool b1 = x-1>=0;
    bool b2 = x+1<WORLD_WIDTH;
    bool b3 = y-1>=0;
    bool b4 = y+1<WORLD_HEIGHT;

    int ct = 0;
    if(b1) ct+= tmp[x-1][y];
    if(b2) ct+= tmp[x+1][y];
    if(b3) ct+= tmp[x][y-1];
    if(b4) ct+= tmp[x][y+1];
    if(b1 && b3) ct+= tmp[x-1][y-1];
    if(b1 && b4) ct+= tmp[x-1][y+1];
    if(b2 && b3) ct+= tmp[x+1][y-1];
    if(b2 && b4) ct+= tmp[x+1][y+1];
    if(ct == 3 || (tmp[x][y] == 1 && ct == 2)){
        board[x][y] = 1;
        q.push({x,y});
    }else
        board[x][y] = 0;
}

void gof_update(){
    int dirs[] = {-1,0, 1,0, 0,-1, 0,1, -1,-1, -1,1, 1,-1, 1,1};
    tmp = board;
    added.clear();

    pair<int, int> endP = {-1, -1};
    q.push(endP);
    while(q.front() != endP){
        pair<int,int> cds = q.front();
        q.pop();
        gof_step(cds);
        for(int i=0; i<16; i+=2)
            gof_step({cds.first + dirs[i], cds.second + dirs[i+1]});
    }
    q.pop();
    added.clear();
}


int main(int argc, char *argv[]){
    for(int i=0; i<WORLD_WIDTH; i++) board[i].resize(WORLD_HEIGHT);


    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "error initializing SDL:" << SDL_GetError() << '\n';
    }
    SDL_Window* win = SDL_CreateWindow("Game of life", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);
 
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);


    int mouseX, mouseY, pmouseX, pmouseY;
    bool lpressed = 0, rpressed = 0, mpressed = 0;
    float offsetX = (WORLD_WIDTH*CELL_PXEL_SIZE-SCREEN_WIDTH)/2;
    float offsetY = (WORLD_HEIGHT*CELL_PXEL_SIZE-SCREEN_HEIGHT)/2;
    int x = 0, y = 0, sdx = offsetX/CELL_PXEL_SIZE, sdy = offsetY/CELL_PXEL_SIZE;
    bool paused = 0;
    int counter = 0;
 
    // controls animation loop
    int close = 0;
 
    // animation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_P:
                    if(paused) paused = 0;
                    else paused = 1;
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    //Up key
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    //Left key
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    //Down key
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    //Right key
                    break;
                default:
                    break;
                }
                break;

            case SDL_MOUSEMOTION:
                pmouseX = mouseX;
                pmouseY = mouseY;
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                if(mpressed){
                    offsetX -= mouseX-pmouseX;
                    offsetY -= mouseY-pmouseY;

                    offsetX = min(offsetX, (float)WORLD_WIDTH*CELL_PXEL_SIZE-SCREEN_WIDTH);
                    offsetY = min(offsetY, (float)WORLD_HEIGHT*CELL_PXEL_SIZE-SCREEN_HEIGHT);
                    offsetX = max(offsetX, (float)0);
                    offsetY = max(offsetY, (float)0);

                    x = -(int)round(offsetX)%CELL_PXEL_SIZE;
                    y = -(int)round(offsetY)%CELL_PXEL_SIZE;

                    sdx = offsetX/CELL_PXEL_SIZE;
                    sdy = offsetY/CELL_PXEL_SIZE;
                }

                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT) lpressed = 1;
                else if(event.button.button == SDL_BUTTON_RIGHT) rpressed = 1;
                else if(event.button.button == SDL_BUTTON_MIDDLE) mpressed = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT) lpressed = 0;
                else if(event.button.button == SDL_BUTTON_RIGHT) rpressed = 0;
                else if(event.button.button == SDL_BUTTON_MIDDLE) mpressed = 0;
                break;
            }
        }

        if(!paused){
        counter++;
            if(counter == 4){
                gof_update();
                counter = 0;
            }
        }

        if(lpressed){
            int pX = (mouseX - x)/CELL_PXEL_SIZE, pY = (mouseY - y)/CELL_PXEL_SIZE;
            if(!board[pX+sdx][pY+sdy]){
                board[pX+sdx][pY+sdy] = 1;
                pair<int,int> pr = {pX+sdx, pY+sdy};
                if(added.find(hashP(pr)) == added.end()){
                    q.push(pr);
                    added.insert(hashP(pr));
                }
            }
        }else if(rpressed){
            int pX = (mouseX - x)/CELL_PXEL_SIZE, pY = (mouseY - y)/CELL_PXEL_SIZE;
            board[pX+sdx][pY+sdy] = 0; 
        }

        // clears the screen
        SDL_SetRenderDrawColor(rend, 30, 30, 30, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 50, 50, 50, SDL_ALPHA_OPAQUE);
        for(int i=x; i<=SCREEN_WIDTH; i+=CELL_PXEL_SIZE)
            SDL_RenderDrawLine(rend, i, 0, i, SCREEN_HEIGHT);
        for(int i=y; i<=SCREEN_HEIGHT; i+=CELL_PXEL_SIZE)
            SDL_RenderDrawLine(rend, 0, i, SCREEN_WIDTH, i);
        SDL_SetRenderDrawColor(rend, 204, 204, 193, SDL_ALPHA_OPAQUE);
        for(int i=0; i<SCREEN_WIDTH/CELL_PXEL_SIZE; i++)
            for(int j=0; j<SCREEN_HEIGHT/CELL_PXEL_SIZE; j++)
                if(board[i+sdx][j+sdy]){
                    SDL_Rect sq;
                    sq.x = x+i*CELL_PXEL_SIZE;
                    sq.y = y+j*CELL_PXEL_SIZE;
                    sq.w = CELL_PXEL_SIZE;
                    sq.h = CELL_PXEL_SIZE;
                    SDL_RenderFillRect(rend, &sq);
                }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
    
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
 
    return 0;
}