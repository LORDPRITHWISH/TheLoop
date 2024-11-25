#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const float grdWidth = 400.0f;
const float grdHeight = 30.0f;

const float dynWidth = 10.0f;
const float dynHeight = 10.0f;

const float SCALE = 1.0f; // Pixels per meter for rendering


struct balldetails{
    b2BodyId ballId;
    float radius;
    int color[4];
}; 

// Convert Box2D coordinates to SDL screen coordinates
int worldToScreenX(float x, float width) {
    // if(width <15)
        // printf("x: %f, width: %f = res: %f \n", x, width, (x - width) * SCALE);
    // printf("x: %f, width: %f = res: %f \n", x, width, (x) * SCALE);
    return static_cast<int>((x - width) * SCALE );
    // return static_cast<int>((x) * SCALE );
}

int worldToScreenY(float y, float height) {
    // if(height <15)

        // printf("y: %f, height: %f = res: %f \n", y, height, SCREEN_HEIGHT - ((y) * SCALE));
        // printf("y: %f, height: %f = res: %f \n", y, height, SCREEN_HEIGHT - ((y + height) * SCALE));
    // return static_cast<int>(SCREEN_HEIGHT - ((y) * SCALE));
    return static_cast<int>(SCREEN_HEIGHT - ((y + height) * SCALE));
}

void createWall(b2WorldId worldId, float x, float y, float width, float height) {
    b2BodyDef wallBodyDef = b2DefaultBodyDef();
    wallBodyDef.position = (b2Vec2){x, y};
    wallBodyDef.type = b2_staticBody;
    // wallBodyDef.restitution = 0.2f;

    b2BodyId wallId = b2CreateBody(worldId, &wallBodyDef);

    b2Polygon wallBox = b2MakeBox(width, height);

    b2ShapeDef wallShapeDef = b2DefaultShapeDef();
    wallShapeDef.density = 100.0f;
    wallShapeDef.friction = 0.3f;
    wallShapeDef.restitution = 1.0f;
    b2CreatePolygonShape(wallId, &wallShapeDef, &wallBox);
}


void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int offsetX, offsetY, d;
    int status;

    offsetX = 0;
    offsetY = radius;
    d = radius - 1;
    status = 0;

    while (offsetY >= offsetX) {
        SDL_RenderDrawLine(renderer, x - offsetY, y + offsetX, x + offsetY, y + offsetX);
        SDL_RenderDrawLine(renderer, x - offsetX, y + offsetY, x + offsetX, y + offsetY);
        SDL_RenderDrawLine(renderer, x - offsetX, y - offsetY, x + offsetX, y - offsetY);
        SDL_RenderDrawLine(renderer, x - offsetY, y - offsetX, x + offsetY, y - offsetX);

        if (d >= 2 * offsetX) {
            d -= 2 * offsetX + 1;
            offsetX += 1;
        } else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        } else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
}


balldetails create_ball(b2WorldId worldId, float x, float y){

    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        std::cerr << "Invalid ball position: " << x << ", " << y << std::endl;
        x= 400.0f;
        y= 300.0f;
    }

    b2BodyDef ballBodyDef = b2DefaultBodyDef();
    ballBodyDef.position = (b2Vec2){x, y};
    ballBodyDef.type = b2_dynamicBody;

    b2BodyId ballId = b2CreateBody(worldId, &ballBodyDef);

    b2Circle circle;
    circle.radius = rand() % 15 + 5;

    b2ShapeDef ballShapeDef = b2DefaultShapeDef();
    ballShapeDef.density = 10.0f;
    ballShapeDef.friction = 0.3f;
    ballShapeDef.restitution = 2.1f;
    b2CreateCircleShape(ballId, &ballShapeDef, &circle);
    return {ballId, circle.radius, {rand() % 255, rand() % 255, rand() % 255, rand() % 255}};
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
    groundShapeDef.density = 100.0f;
    groundShapeDef.friction = 0.3f;
    groundShapeDef.restitution = 0.2f;
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    createWall(worldId, 0.0f, 0.0f, 1.0f, SCREEN_HEIGHT);
    createWall(worldId, SCREEN_WIDTH-1, 0.0f, 1.0f, SCREEN_HEIGHT);
    // createWall(worldId, 0.0f, 0.0f, SCREEN_WIDTH, 1.0f);
    createWall(worldId, 0.0f, SCREEN_HEIGHT-1, SCREEN_WIDTH, 1.0f);

    // Dynamic body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){50.0f, 500.0f};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    // b2Polygon dynamicBox = b2MakeBox(dynWidth, dynHeight);
    // b2Circle dynamicCircle = b2MakeCircle(dynWidth);
    // b2Circle circle = {{2.0f, 3.0f}, 0.5f};
    b2Circle circle;
    // circle.center = (b2Vec2){50.0f, 500.0f};
    circle.radius = dynWidth;
    // b2BodyId bodyId = b2CreateBody(worldId, &circle);


    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.friction = 0.3f;
    shapeDef.restitution = 0.6f;
    // b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
    b2CreateCircleShape(bodyId, &shapeDef, &circle);

    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;

    bool running = true;
    SDL_Event event;

    std::vector<balldetails> balls;

    // Simulation loop
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
                int moux, mouy;
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        b2Body_ApplyLinearImpulseToCenter(bodyId, (b2Vec2){-2000.0f, 0.0f}, true);
                        break;
                    case SDLK_RIGHT:
                        b2Body_ApplyLinearImpulseToCenter(bodyId, (b2Vec2){2000.0f, 0.0f}, true);
                        break;
                    case SDLK_UP:
                    if (velocity.y <= 10.0f){
                        b2Body_ApplyLinearImpulseToCenter(bodyId, (b2Vec2){0.0f, 20000.0f}, true);
                    }
                        break;
                    case SDLK_DOWN:
                        b2Body_ApplyLinearImpulseToCenter(bodyId, (b2Vec2){0.0f, -2000.0f}, true);
                        break;
                    case SDLK_SPACE:
                        if (abs(velocity.y) <= 50.0f) { // Ensure the box is stationary vertically
                            b2Vec2 jumpImpulse = {0.0f, 500000.0f}; // Impulse vector (x, y)
                            b2Body_ApplyLinearImpulseToCenter(bodyId, jumpImpulse, true);
                        }
                        break;
                    case SDLK_RETURN:
                        for(balldetails ball: balls)
                            b2Body_ApplyLinearImpulseToCenter(ball.ballId, (b2Vec2){(float)(rand() % 1000000)-500000,(float)(rand() % 1000000)-500000}, true);
                        break;
                    case SDLK_KP_0:
                        // balls.push_back(create_ball(worldId, 50.0f, 500.0f));
                        SDL_GetMouseState(&moux, &mouy);
                        // printf("x: %i, y: %i \n", moux, mouy);
                        balls.push_back(create_ball(worldId, moux, SCREEN_HEIGHT - mouy));
                        break;
                    case SDLK_q:
                        balls.push_back(create_ball(worldId, rand() % SCREEN_WIDTH, SCREEN_HEIGHT - (rand() % 300)));
                        break;
                    case SDLK_BACKSPACE:
                        for(balldetails ball: balls)
                            b2DestroyBody(ball.ballId);
                        balls.clear();
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;

                    default:
                        break;
                }

            }

            if(event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.x < 10 || event.button.x > SCREEN_WIDTH - 10 || event.button.y < 10 || event.button.y > SCREEN_HEIGHT - 10)
                    continue;
                if(event.button.button == SDL_BUTTON_LEFT)
                    balls.push_back(create_ball(worldId, event.button.x, SCREEN_HEIGHT - event.button.y));
                if(event.button.button == SDL_BUTTON_RIGHT)
                    for (auto it = balls.begin(); it != balls.end(); ++it) {
                        int ballX = worldToScreenX(b2Body_GetPosition(it->ballId).x, 0);
                        int ballY = worldToScreenY(b2Body_GetPosition(it->ballId).y, 0);
                        int ballRadius = static_cast<int>(it->radius * SCALE);

                        int dx = event.button.x - ballX;
                        int dy = event.button.y - ballY;
                        if (dx * dx + dy * dy <= ballRadius * ballRadius) {
                            b2DestroyBody(it->ballId);
                            balls.erase(it);
                            break;
                        }
                    }
                // if (event.button.button == SDL_BUTTON_MIDDLE)

            }
        }
        // printf("velocity: %f \n", b2Body_GetLinearVelocity(bodyId).y);



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
        int circleX = worldToScreenX(dynamicPos.x, 0);
        int circleY = worldToScreenY(dynamicPos.y, 0);
        int circleRadius = static_cast<int>(dynWidth * SCALE);

        SDL_RenderDrawCircle(renderer, circleX, circleY, circleRadius);

        for(balldetails ball: balls){
            SDL_SetRenderDrawColor(renderer, ball.color[0], ball.color[1], ball.color[2], ball.color[3]);
            int circleX = worldToScreenX(b2Body_GetPosition(ball.ballId).x, 0);
            int circleY = worldToScreenY(b2Body_GetPosition(ball.ballId).y, 0);
            int circleRadius = static_cast<int>(ball.radius * SCALE);
            SDL_RenderDrawCircle(renderer, circleX, circleY, circleRadius);
        }


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
