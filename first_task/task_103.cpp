#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CIRCLE_RADIUS 50 
#define CIRCLE_SPEED 3 // circle speed for which circle that move

bool gameIsRunning = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int circle1X = 0; // x-coordinate of the first circle
int circle1Y = SCREEN_HEIGHT / 2; // y-coordinate of the first circle
int circle2X = SCREEN_WIDTH / 2; // x-coordinate of the second circle
int circle2Y = 0; // y-coordinate of the second circle
bool upKeyPressed = false;
bool downKeyPressed = false;
bool leftKeyPressed = false;       
bool rightKeyPressed = false;

// Define sdl_color structure  to change the circles color
SDL_Color circle1Color = {255, 0, 0, 255}; // Red
SDL_Color circle2Color = {0, 0, 255, 255}; // Blue



// process event function
void processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:    // check are you press any key or not
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        upKeyPressed = true;            // if press any key its asign true
                        break;
                    case SDLK_DOWN:
                        downKeyPressed = true;
                        break;
                    case SDLK_LEFT:
                        leftKeyPressed = true;
                        break;
                    case SDLK_RIGHT:
                        rightKeyPressed = true;
                        break;
                }
                break;
            case SDL_KEYUP:                            // for not press key false
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        upKeyPressed = false;
                        break;
                    case SDLK_DOWN:
                        downKeyPressed = false;
                        break;
                    case SDLK_LEFT:
                        leftKeyPressed = false;
                        break;
                    case SDLK_RIGHT:
                        rightKeyPressed = false;
                        break;
                }
                break;
        }
    }
}

// draw the circle

void drawCircles() {
    // Clear the window screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the first circle
    SDL_SetRenderDrawColor(renderer, circle1Color.r, circle1Color.g, circle1Color.b, circle1Color.a);
    for (int x = -CIRCLE_RADIUS; x <= CIRCLE_RADIUS; x++) {
        for (int y = -CIRCLE_RADIUS; y <= CIRCLE_RADIUS; y++) {
            if (x * x + y * y <= CIRCLE_RADIUS * CIRCLE_RADIUS) {
                SDL_RenderDrawPoint(renderer, circle1X + x, circle1Y + y);
            }
        }
    }

    // Draw the second circle
    SDL_SetRenderDrawColor(renderer, circle2Color.r, circle2Color.g, circle2Color.b, circle2Color.a);
    for (int x = -CIRCLE_RADIUS; x <= CIRCLE_RADIUS; x++) {
        for (int y = -CIRCLE_RADIUS; y <= CIRCLE_RADIUS; y++) {
            if (x * x + y * y <= CIRCLE_RADIUS * CIRCLE_RADIUS) {
                SDL_RenderDrawPoint(renderer, circle2X + x, circle2Y + y);
            }
        }
    }

    // Present the rendered content
    SDL_RenderPresent(renderer);
}


//for moving the one circle
void moveCircles() {
    // Move the first circle from left to right continuously
    circle1X = (circle1X + CIRCLE_SPEED) % SCREEN_WIDTH;

    // Move the second circle based on key inputs
    if (upKeyPressed) {
        circle2Y = (circle2Y - CIRCLE_SPEED + SCREEN_HEIGHT) % SCREEN_HEIGHT;
    }
    if (downKeyPressed) {
        circle2Y = (circle2Y + CIRCLE_SPEED) % SCREEN_HEIGHT;
    }
    if (leftKeyPressed) {
        circle2X = (circle2X - CIRCLE_SPEED + SCREEN_WIDTH) % SCREEN_WIDTH;
    }
    if (rightKeyPressed) {
        circle2X = (circle2X + CIRCLE_SPEED) % SCREEN_WIDTH;
    }
}


// function for checking if collision is happen or not between them
bool checkCollision() {
    // Calculate distance between the centers of the circles
    int distanceSquared = (circle1X - circle2X) * (circle1X - circle2X) +
                          (circle1Y - circle2Y) * (circle1Y - circle2Y);
    int radiusSquared = (CIRCLE_RADIUS + CIRCLE_RADIUS) * (CIRCLE_RADIUS + CIRCLE_RADIUS);

    // Check if collision occurred
    if (distanceSquared <= radiusSquared) {
        return true;
    }
    return false;
}


// manage the collision
void manageCollision()
{
    circle2X = SCREEN_WIDTH / 2;
    circle2Y = 0;
}

bool flage=true;
void visualEffect() {
    // Change the colors of the circles upon collision
    
    if(flage){
        
    circle1Color = {0, 255, 0, 255}; // Green
    circle2Color = {255, 255, 0, 255}; // Yellow
    }
    else{
        circle1Color = {255, 0, 0, 255}; // Red
         circle2Color = {0, 0, 255, 255}; // Blue
    }
    flage=!flage;
}


void destroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//write main function and initialize window and render into the main fuction
int main(int argc, char **argv) {
    gameIsRunning = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Task_103 :project work- 1st Task",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    


    while (gameIsRunning) {
        processInput();
        moveCircles();
        if (checkCollision()) {
            //manageCollision();                // you can uncomment this line check this for start the circle2 from begening
            visualEffect();
        }
        drawCircles();
        SDL_Delay(16); 
    }

    return 0;
}
