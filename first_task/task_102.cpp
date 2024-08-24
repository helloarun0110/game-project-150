#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>

// below I define window size and some constant
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define INITIAL_RADIUS 50

bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Uint32 startTime;
Uint32 currentTime;
int radius = INITIAL_RADIUS;


// same like first task101 write initialization function
bool initializeWindow(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Task_102 :project work- 1st Task",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }
    return true;
}


// to process input
void processInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameIsRunning = false;
            break;

        default:
            break;
        }
    }
}


// this function to create circle and change the radius respect to time 

void drawCircle(int centerX, int centerY, int radius)
{
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}


void draw(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    currentTime = SDL_GetTicks();
    radius = INITIAL_RADIUS + (currentTime - startTime) / 10;
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
                                                                           // // here the main changes from the 101task
    if (centerX + radius >= SCREEN_WIDTH || centerX - radius <= 0 ||
        centerY + radius >= SCREEN_HEIGHT || centerY - radius <= 0)        // when radius is big the window size it start from the first
    {
        radius = INITIAL_RADIUS;
        startTime = currentTime;    //for start from the first
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawCircle(centerX, centerY, radius);

    SDL_RenderPresent(renderer);
}

// write destroy function 
void destroyWindow(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// now time to write the main fuction

int main(int argc, char **argv)
{
    gameIsRunning = initializeWindow();
    startTime = SDL_GetTicks(); // count time from runnig the window

    while (gameIsRunning)    //game loop
    {
        processInput(); 
        draw();
    }

    destroyWindow();

    return 0;
}
