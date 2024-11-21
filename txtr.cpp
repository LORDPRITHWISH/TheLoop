#include<iostream>
#include<SDL2/SDL.h>

int main(int argc,char**args){
    SDL_Window* window;
    SDL_Renderer* renderer;



    int result = SDL_CreateWindowAndRenderer( 640, 580, 0, &window, &renderer );
    if ( result != 0 ) {
        std::cout << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
    }
    SDL_Surface* image = SDL_LoadBMP("../assets/RAY.BMP");

    if(image == NULL){
        std::cout << "Failed to load image: " << SDL_GetError() << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,image);

    if(texture == NULL){
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderDrawPoint( renderer, 100, 100 );

    // Update window
    // SDL_RenderPresent( renderer );
    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_ADD );

    SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD ); // Switch to additive blending

    SDL_RenderDrawLine( renderer, 0, 0, 100, 200 ); // Draw with additive blending

    SDL_SetRenderDrawColor( renderer, 10, 100, 255, 255 ); // Draw in solid blue

    SDL_RenderDrawLine( renderer, 10, 10, 50, 25 ); // Draw a line

    SDL_Rect rect;
    rect.x = 150;
    rect.y = 25;
    rect.h = 75;
    rect.w = 120;

    SDL_RenderFillRect( renderer, &rect );
    SDL_SetTextureColorMod(texture, 200, 100, 50);
    SDL_SetTextureAlphaMod( texture, 20 );


    SDL_Rect dst;
    dst.x = 400;
    dst.y = 250;
    dst.w = 200;
    dst.h = 200;

    SDL_RenderCopy( renderer, texture, NULL, &dst );

    unsigned char r, g, b;

    SDL_GetTextureColorMod( texture, &r, &g, &b );

    SDL_SetTextureColorMod( texture, g, b, r );
    
    unsigned char alphaval;
    SDL_GetTextureAlphaMod( texture, &alphaval );
    
    std::cout << "alphaval"<<alphaval<<std::endl;

    // SDL_Rect dst;
    dst.x = 400;
    dst.y = 20;
    dst.w = 200;
    dst.h = 200;

    SDL_SetTextureAlphaMod( texture, 200 );

    SDL_RenderCopy( renderer, texture, NULL, &dst );
    
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

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );

    renderer = NULL;
    window = NULL;
}