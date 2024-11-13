#include<SDL2/SDL.h>
#include<iostream>


bool init();
void kill();
bool load();

// Pointers to our window and surfaces
SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* image1;
SDL_Surface* image2;


int main(int argc, char** argv){

    if (!init()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return 1;
    }

    if (!load()) {
        std::cerr << "Failed to load media" << std::endl;
        return 1;
    }

    // Blit image to entire window
	SDL_BlitSurface( image1, NULL, winSurface, NULL );
    SDL_UpdateWindowSurface( window );

    SDL_Rect dest;
	dest.x = 160;
	dest.y = 120;
	dest.w = 320;
	dest.h = 240;
	SDL_BlitScaled( image2, NULL, winSurface, &dest );

	// Update window
	SDL_UpdateWindowSurface( window );

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    kill();

	return 0;

}
    

bool load() {
	// Temporary surfaces to load images into
		// This should use only 1 temp surface, but for conciseness we use two
	SDL_Surface *temp1, *temp2;

	// Load images
	temp1 = SDL_LoadBMP("../assets/RAY.BMP");
	temp2 = SDL_LoadBMP("../assets/sample_1920×1280.bmp");

	// Make sure loads succeeded
	if ( !temp1 || !temp2 ) {
		std::cout << "Error loading image: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	// Format surfaces
	image1 = SDL_ConvertSurface( temp1, winSurface->format, 0 );
	image2 = SDL_ConvertSurface( temp2, winSurface->format, 0 );

	// Free temporary surfaces
	SDL_FreeSurface( temp1 );
	SDL_FreeSurface( temp2 );

	// Make sure format succeeded
	if ( !image1 || !image2 ) {
		std::cout << "Error converting surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}
	return true;
}

bool init() {
	// See last example for comments
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	} 

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
	if ( !window ) {
		std::cout << "Error creating window: " << SDL_GetError()  << std::endl;
		system("pause");
		return false;
	}

	winSurface = SDL_GetWindowSurface( window );
	if ( !winSurface ) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}
	return true;
}

void kill() {
	// Free images
	SDL_FreeSurface( image1 );
	SDL_FreeSurface( image2 );

	// Quit
	SDL_DestroyWindow( window );
	SDL_Quit();
}