#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <cmath>

// Vertex data for a cube
GLfloat vertices[] = {
    // Positions           // Colors
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  // Bottom-left-back
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  // Bottom-right-back
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  // Top-right-back
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  // Top-left-back
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  // Bottom-left-front
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  // Bottom-right-front
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,  // Top-right-front
    -0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f   // Top-left-front
};

// Indices for the cube (6 faces)
GLuint indices[] = {
    0, 1, 2, 2, 3, 0,  // Back face
    4, 5, 6, 6, 7, 4,  // Front face
    0, 4, 7, 7, 3, 0,  // Left face
    1, 5, 6, 6, 2, 1,  // Right face
    3, 7, 6, 6, 2, 3,  // Top face
    0, 4, 5, 5, 1, 0   // Bottom face
};

void handleInput(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow(
        "3D Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    // Create an OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up vertex data
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Main loop
    bool running = true;
    while (running) {
        handleInput(running);

        // Clear screen
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
