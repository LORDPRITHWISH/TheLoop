#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float grdWidth = 400.0f;
const float grdHeight = 50.0f;

const float dynWidth = 10.0f;
const float dynHeight = 10.0f;

const float SCALE = 1.0f; // Pixels per meter for rendering

// Convert Box2D coordinates to SDL screen coordinates
int worldToScreenX(float x, float width) {
    printf("x: %f, width: %f = res: %f \n", x, width, (x - width) * SCALE);
    // printf("x: %f, width: %f = res: %f \n", x, width, (x) * SCALE);
    return static_cast<int>((x - width) * SCALE );
    // return static_cast<int>((x) * SCALE );
}

int worldToScreenY(float y, float height) {
    // printf("y: %f, height: %f = res: %f \n", y, height, SCREEN_HEIGHT - ((y) * SCALE));
    printf("y: %f, height: %f = res: %f \n", y, height, SCREEN_HEIGHT - ((y + height) * SCALE));
    // return static_cast<int>(SCREEN_HEIGHT - ((y) * SCALE));
    return static_cast<int>(SCREEN_HEIGHT - ((y + height) * SCALE));
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Box2D Simulation with SDL",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize the gravity vector for the world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -100.0f};

    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Ground body
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    // groundBodyDef.position = (b2Vec2){0.0f+grdWidth, 0.0f+grdHeight};
    groundBodyDef.position = (b2Vec2){0.0f+grdWidth, 0.0f+grdHeight};

    b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
    b2Polygon groundBox = b2MakeBox(grdWidth, grdHeight);

    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.density = 0.0f;
    groundShapeDef.friction = 0.3f;
    groundShapeDef.restitution = 0.8f;
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    // Dynamic body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){50.0f, 500.0f};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    b2Polygon dynamicBox = b2MakeBox(dynWidth, dynHeight);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;
    b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;

    bool running = true;
    SDL_Event event;

    // Simulation loop
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Step the physics world
        b2World_Step(worldId, timeStep, subStepCount);

        // Get positions
        b2Vec2 groundPos = b2Body_GetPosition(groundId);
        b2Vec2 dynamicPos = b2Body_GetPosition(bodyId);

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the ground
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect groundRect = {
            worldToScreenX(groundPos.x, grdWidth),
            worldToScreenY(groundPos.y, grdHeight),
            static_cast<int>(grdWidth * SCALE * 2),
            static_cast<int>(grdHeight * SCALE * 2)
        };
        SDL_RenderFillRect(renderer, &groundRect);

        // Draw the dynamic box
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect dynamicRect = {
            worldToScreenX(dynamicPos.x , dynWidth),
            worldToScreenY(dynamicPos.y , dynHeight),
            static_cast<int>(dynWidth * SCALE * 2),
            static_cast<int>(dynHeight * SCALE * 2)
        };
        SDL_RenderFillRect(renderer, &dynamicRect);

        // Present the updated screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // Clean up
    b2DestroyWorld(worldId);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
