#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<SDL2/SDL.h>






enum Direction {k_up, k_down, k_left, k_right};

struct Snake{
    enum Direction direction;
    float speed;
    int size;
    bool alive;
    float head_x;
    float head_y;
    SDL_Point body[1024];     // I fixed the snake body
    int body_length;
    int growing;
    int grid_width;
    int grid_height;

};



struct Snake snake;
SDL_Point food;
int score = 0;






void update_head(){
    switch(snake.direction){
        case k_up:
            snake.head_y -= snake.speed;
            break;
        
        case k_down:
            snake.head_y += snake.speed;
            break;
        
        case k_left:
            snake.head_x -= snake.speed;
            break;
        
        case k_right:
            snake.head_x += snake.speed;
            break;

    }
    
    snake.head_x = fmod(snake.head_x + snake.grid_width, snake.grid_width );
    snake.head_y = fmod(snake.head_y + snake.grid_height , snake.grid_height);

}



void update_body(SDL_Point current_head_cell, SDL_Point prev_head_cell){
    snake.body[snake.body_length] = prev_head_cell;
    snake.body_length ++;

    if(!snake.growing){
        for(int i=1; i< snake.body_length; i++){
            snake.body[i-1] = snake.body[i];
        }
        snake.body_length --;
    }
    else{
        snake.growing = false;
        snake.size ++;
        snake.speed -= 0.01f;
    }

    for(int i =0; i< snake.body_length; i++){
        if(current_head_cell.x == snake.body[i].x && current_head_cell.y == snake.body[i].y){
            snake.alive= false;
        }
    }


}



void snake_update(){
    SDL_Point prev_cell = {(int) snake.head_x, (int) snake.head_y};
    update_head();
    
    SDL_Point curr_cell = {(int)snake.head_x, (int)snake.head_y};
    if(curr_cell.x != prev_cell.x || curr_cell.y != prev_cell.y){
        update_body(curr_cell, prev_cell);
    }

}



void grow_body(){
    snake.growing = true;

}



bool snake_cell(int x, int y){
    if ( x== (int) snake.head_x && y== (int)snake.head_y){
        return true;

    }

    for(int i=0; i< snake.body_length; i++){
        if (x== snake.body[i].x && y == snake.body[i].y){
            return true;
        }
    }

    return false;

}


void place_food(){

    int x, y;
    while(true){
        x = rand() % snake.grid_width;
        y = rand() % snake.grid_height;

        if(!snake_cell(x,y)){
            food.x = x;
            food.y = y;
            return ;
        }
    }

}




void game_update(){
    if(!snake.alive) return;

    snake_update();

    int new_x = (int)snake.head_x;
    int new_y = (int)snake.head_y;

    if(food.x == new_x && food.y == new_y){
        score ++;
        place_food();
        grow_body();
        snake.speed += 0.02f;
    }
}




void handel_input(bool * running){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            *running = false;
        }
        else if (e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){
                case SDLK_UP:
                    if(snake.direction != k_down || snake.size == 1){
                        snake.direction = k_up;
                    }
                    break;
                
                case SDLK_DOWN:
                    if (snake.direction != k_up || snake.size == 1){
                        snake.direction = k_down;
                    }
                    break;

                case SDLK_LEFT:
                    if(snake.direction != k_right || snake.size == 1){
                        snake.direction = k_left;
                    }
                    break;

                case SDLK_RIGHT:
                    if (snake.direction != k_left || snake.size == 1){
                        snake.direction = k_right;
                    }
                    break;

            }
        }
    }
    
}




void render(SDL_Renderer * sdl_renderer, const int screen_width, const int screen_height, const int grid_width, const int grid_height){
    SDL_Rect block;

    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;
    block.x = food.x * block.w;
    block.y = food.y * block.h;

    SDL_SetRenderDrawColor(sdl_renderer, 16,21,30,255);
    SDL_RenderClear(sdl_renderer);
    SDL_SetRenderDrawColor(sdl_renderer, 255, 204, 0, 255);

    SDL_RenderFillRect(sdl_renderer, &block);

    if(snake.alive){
        SDL_SetRenderDrawColor(sdl_renderer, 0, 122, 204, 255);

    }
    else{
        SDL_SetRenderDrawColor(sdl_renderer, 16, 21, 30, 255);
    }

    for (int i=0; i< snake.body_length; i++){
        block.x= snake.body[i].x * block.w;
        block.y = (int)snake.head_y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
        SDL_RenderPresent(sdl_renderer);
    }

    block.x = (int)snake.head_x * block.w;
    block.y = (int)snake.head_y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
    SDL_RenderPresent(sdl_renderer);

}



void update_window_title(SDL_Window * sdl_window, int score, int fps){
    char title[64];
    snprintf(title, sizeof(title), "snake score: %d  fps: %d",score,fps);
    SDL_SetWindowTitle(sdl_window, title);


}


void run_game(SDL_Window *sdl_window, SDL_Renderer *sdl_renderer, const int target_frame_duration, const int screen_width, const int screen_height, const int grid_width, const int grid_height){
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    Uint32 frame_count =0;
    bool running = true;

    while(running){

        frame_start = SDL_GetTicks();
        handel_input(&running);
        game_update();
        render(sdl_renderer, screen_width, screen_height, grid_width, grid_height);
        if(!snake.alive){
            running = false;
        }

        frame_end = SDL_GetTicks();
        frame_count ++;
        frame_duration = frame_end - frame_start;
        if(frame_end - title_timestamp >= 1000){
            update_window_title(sdl_window, score, frame_count);
            frame_count =0;
            title_timestamp = frame_end;

        }

        if(frame_duration < target_frame_duration){
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }


}




int main(int argc, char* argv[]){

const int frame_per_second = 60;
const int ms_per_frame = 1000/ frame_per_second;
const int screen_width = 640;
const int screen_height = 640;
const int grid_width =32;
const int grid_height =32;




snake.grid_width = grid_width;
snake.grid_height = grid_height;
snake.head_x = grid_width / 2;
snake.head_y = grid_height /2;
snake.direction = k_up;
snake.speed = 0.1f;
snake.alive = true;
snake.body_length = 0;
snake.growing = false;



if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("error : sdl failed to initialize\n sdl error:'%s'",SDL_GetError());
    return 1;
}


SDL_Window *sdl_window = SDL_CreateWindow("snake game",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            screen_width,
                                            screen_height,
                                            SDL_WINDOW_SHOWN);




if(sdl_window == NULL){
    printf("error: window could not create\n sdl_error:'%s'", SDL_GetError());
    return 1;
}


SDL_Renderer *sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
if(sdl_renderer == NULL){
    printf("error: renderer could not create\n  sdl_error: '%s'", SDL_GetError());
    return 1;
}




place_food();
run_game(sdl_window , sdl_renderer, ms_per_frame, screen_width, screen_height, grid_width, grid_height);


printf("game has terminated successfully!\n");
printf("score: %d\n",score);
printf("size: %d\n",snake.size);





SDL_DestroyRenderer(sdl_renderer);
SDL_DestroyWindow(sdl_window);
SDL_Quit();

return 0;

}
