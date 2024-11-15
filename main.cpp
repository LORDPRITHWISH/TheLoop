#include<iostream>
#include<SDL2/SDL.h>

int main(int argc,char**args){
    SDL_Window* window;
    SDL_Renderer* renderer;

    int result = SDL_CreateWindowAndRenderer( 640, 480, 0, &window, &renderer );
    if ( result != 0 ) {
        std::cout << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
    }
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderDrawPoint( renderer, 100, 100 );

// Update window
    // SDL_RenderPresent( renderer );

    SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD ); // Switch to additive blending

    SDL_RenderDrawLine( renderer, 0, 0, 100, 200 ); // Draw with additive blending
    
    SDL_RenderPresent( renderer );

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

}