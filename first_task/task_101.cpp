#include<SDL2/SDL.h>
#include<stdio.h>
#include<cmath>

// here I define the window size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//here I define the constant and global pointer
bool gameIsRunning =false;
SDL_Window *window=NULL;
SDL_Renderer *renderer=NULL;
const int circle_radius=100;

// write function for window and renderer initializetion
bool initializeWindow(void)
{
    if (SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("error: sdl failed to initialize\nsdl error is: '%s'\n",SDL_GetError());
        return false;
    }

    window=SDL_CreateWindow(
        "Task_101 :project work- 1st Task",   // its my window title project name
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if(!window)
    {
        printf("error failed to open windoe\nsdl error is :'%s'",SDL_GetError()); // this function return the actuall error
        return false;
    }

    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        printf("error to create renderer\nsdl error is :'%s'\n",SDL_GetError());
        return false;
    }

    return true;
}

// now I try to write function for process input
void processInput(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                gameIsRunning=false;
                break;
            
            default:
                break;
        }
    }
}

// now we need a function to do the task of draw a circle here I sud draw function
void drawCircle(int centerX, int centerY, int radius)
{
    for(int i =-radius;i<=radius;i++)
    {
        for(int j =-radius;j<=radius;j++)
        {
            if (i*i +j*j <=radius*radius)  // circle create contion   X^2 + Y^2 = r^2
            {
                SDL_RenderDrawPoint(renderer,centerX+i,centerY+j);
            }
        }
    }
}

//here write main draw function
void draw(void)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);  // you can think window is a CANVAS and rander is a paint brush here I 
    SDL_RenderClear(renderer);                   // full canvas paint black to draw circle

    int radius=circle_radius;
    int centerX=SCREEN_WIDTH/2;    // to create X and Y corrdinate at the mindle of window
    int centerY=SCREEN_HEIGHT/2;


    SDL_SetRenderDrawColor(renderer,255,255,255,255);  // now paint the circle in white
    drawCircle(centerX,centerY,radius);

    SDL_RenderPresent(renderer);  //show the paint on window
}


// for close all function and releae all graphics and memmory
void destroyWindow()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// now it time to wirte main function and call the all sub function
int main(int argc, char **argv)
{
    gameIsRunning=initializeWindow();

    // it is  main game loop
    while(gameIsRunning)
    {
        processInput();
        draw();
    }

// for close all function and releae all graphics and memmory
    destroyWindow();
  return 0;  
}
