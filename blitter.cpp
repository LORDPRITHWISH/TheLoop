#include<SDL2/SDL.h>
#include<iostream>

int main(int argc, char** argv){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1210, 720, SDL_WINDOW_SHOWN);

    if ( !win ) {
        std::cout << "Failed to create a window! Error: " << SDL_GetError() << std::endl; 
    }

    SDL_Surface* winSurface = SDL_GetWindowSurface( win );

    SDL_Surface* image = SDL_ConvertSurface(SDL_LoadBMP("../assets/RAY.BMP"), winSurface->format, 0);

    if ( !image ) {
        std::cout << "Error loading image: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Rect des;
    des.h = 100;
    des.w = 100;
    des.x = 50;
    des.y = 50;

    SDL_BlitScaled(image ,NULL,winSurface,&des);

    // SDL_BlitSurface(image, NULL, winSurface, NULL);
    
    SDL_UpdateWindowSurface( win );

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        Uint32 startTick = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_KP_PLUS]) {
            des.h+=2;
            des.w+=2;
        }
        if (currentKeyStates[SDL_SCANCODE_KP_MINUS]) {
            des.h-=2;
            des.w-=2;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN])
            des.y+=1;
        if (currentKeyStates[SDL_SCANCODE_UP])
            des.y-=1;
        if (currentKeyStates[SDL_SCANCODE_RIGHT])
            des.x+=1;
        if (currentKeyStates[SDL_SCANCODE_LEFT])
            des.x-=1;
        
        // SDL_SetRenderDrawColor(winSurface, 255, 255, 255, 255);
        SDL_BlitScaled(image ,NULL,winSurface,&des);
        SDL_UpdateWindowSurface( win );
        
        Uint32 frameDuration = SDL_GetTicks() - startTick;
        // std::cout << "Frame Duration: " << frameDuration << "ms" << std::endl;
        if (frameDuration < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameDuration);
        }

    }

}