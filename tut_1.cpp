#include<SDL2/SDL.h>
#include<iostream>

int main(int argc, char** argv){
    
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window* win = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    if ( !win ) {
	    std::cout << "Failed to create a window! Error: " << SDL_GetError() << std::endl; 
    }

    SDL_Surface* winSurface = SDL_GetWindowSurface( win );


    
    SDL_FillRect( winSurface, NULL, SDL_MapRGB( winSurface->format, 255, 90, 120 ));
    
    SDL_UpdateWindowSurface( win );


    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow( win );


    win = NULL;
    winSurface = NULL;

    SDL_Quit();

    return 0;
}