#include <SDL2/SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int RECT_SIZE = 50;
const int SPEED = 1;

int main(int argc, char* argv[]) {



    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Multiple Key Presses", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Rectangle position
    int posX = SCREEN_WIDTH / 2 - RECT_SIZE / 2;
    int posY = SCREEN_HEIGHT / 2 - RECT_SIZE / 2;

    bool quit = false;
    SDL_Event event;

    // Main game loop
    while (!quit) {
        Uint32 startTick = SDL_GetTicks();

        // Handle quit events
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) quit = true;
        }

        // Get the state of all keys
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        // Check for key combinations for simultaneous movement
        if (currentKeyStates[SDL_SCANCODE_UP] && posY > 0) {
            posY -= SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN] && posY < SCREEN_HEIGHT - RECT_SIZE) {
            posY += SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT] && posX > 0) {
            posX -= SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] && posX < SCREEN_WIDTH - RECT_SIZE) {
            posX += SPEED;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw rectangle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect fillRect = { posX, posY, RECT_SIZE, RECT_SIZE };
        SDL_RenderFillRect(renderer, &fillRect);

        // Uint32 startTick = SDL_GetTicks();

        SDL_RenderPresent(renderer);

        Uint32 frameDuration = SDL_GetTicks() - startTick;
        // std::cout << "Frame Duration: " << frameDuration << "ms" << std::endl;
        if (frameDuration < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameDuration);
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
