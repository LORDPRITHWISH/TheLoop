cmake_minimum_required(VERSION 3.10)
project(SDLEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Include OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Include GLEW
find_library(GLEW_LIB GLEW REQUIRED HINTS /usr/lib/x86_64-linux-gnu)
include_directories(/usr/include/GL)

# Add the executable
add_executable(game main.cpp)

# Link libraries
target_link_libraries(game ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIB})
