#include<stdio.h>

#include<SDL2/SDL.h>

#include<math.h>

int SCREEN_WIDTH = 800;

int SCREEN_HEIGHT = 600;

SDL_Window* window=NULL;

SDL_Renderer* renderer=NULL;

bool game_running=false;

int rad1 = 70;

int cenx1 = -rad1;

int ceny1 = SCREEN_HEIGHT/2;

int rad2 = 30;

int cenx2 = SCREEN_WIDTH/2;

int ceny2 = rad2;

bool initialize_window()
{
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        printf("ERROR INITIALIZING SDL!");
        return false;
    }

    window=SDL_CreateWindow("Created by Fatema 07", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH , SCREEN_HEIGHT, 0);
    
    if(!window)
    {
         return false;
    }
    renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        return false;
    }

    return true;
}

void setup()
{
    SDL_SetRenderDrawColor(renderer, 70, 80, 90, 255);
    SDL_RenderClear(renderer);
    SDL_Color color = {0,0,0,0};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

}
void process_input()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
         if(event.type == SDL_KEYDOWN)//to move circle pressing keys
         {
           if(SDLK_RIGHT==event.key.keysym.sym)
           {
            cenx2 += 5;
           }

            else if(SDLK_LEFT==event.key.keysym.sym)
            {
                cenx2 -= 5;
            }

            else if(SDLK_UP==event.key.keysym.sym)
            {
                ceny2-=5;
            }
            else if(SDLK_DOWN==event.key.keysym.sym)
            {
                ceny2 += 5;
            }
         }

        else if (event.type == SDL_QUIT)
        {
            game_running = false;
        }
    }
}

void drawSolidCircle(SDL_Renderer* renderer, int cenx1, int ceny1, int rad1)
{
    for (int x = -rad1; x <= rad1; x++)
    {
        for (int y = -rad1; y <= rad1; y++)
        {
            if (x * x + y * y <= rad1 * rad1)
            {
                SDL_RenderDrawPoint(renderer, cenx1 + x, ceny1 + y);
            }
        }
    }
}

void update()
{
    cenx1 +=2;

    if(cenx1 - rad1 >= SCREEN_WIDTH)

    cenx1 = -rad1;

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 200, 150, 255);

    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
}
void render()
{
    drawSolidCircle(renderer, cenx1, ceny1, rad1);

    SDL_RenderPresent(renderer);

    drawSolidCircle(renderer, cenx2, ceny2, rad2);

    SDL_RenderPresent(renderer);
}
void collision()
{
    
    int displacement = sqrt((cenx1-cenx2) * (cenx1 - cenx2) + (ceny1-ceny2) * (ceny1-ceny2));
    if(displacement <= rad1 + rad2)
    {
    SDL_Rect r;
    r.x = 0;

    r.y = 0;

    r.w = SCREEN_WIDTH;

    r.h = SCREEN_HEIGHT;

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0,100, 50, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 150, 25, 255);

    SDL_RenderDrawRect(renderer,&r);

    SDL_RenderPresent(renderer);
    }
}

void destroyWindow()
{
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}

int main(int argc, char *argv[])
{
    game_running=initialize_window();

    while(game_running!=false)
    {
        process_input();

        update();

        render();
        
        collision();
    }

   destroyWindow();

  return 0;
}



