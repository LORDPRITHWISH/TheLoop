cmake_minimum_required(VERSION 3.10)
project(SDLEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CMP0072 to use GLVND by default for OpenGL
if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Manually include SDL2_image paths
include_directories(/usr/include/SDL2)
set(SDL2_IMAGE_LIB /usr/lib/x86_64-linux-gnu/libSDL2_image.so)

# Include OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Include GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Set Box2D paths manually
set(BOX2D_INCLUDE_DIR "/usr/local/include/box2d")
set(BOX2D_LIB "/usr/local/lib/libbox2d.a")  # or libbox2d.so if using shared libraries

include_directories(${BOX2D_INCLUDE_DIR})

# Add the executable
add_executable(game main.cpp)

# Link libraries
target_link_libraries(game ${SDL2_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIB} ${BOX2D_LIB} ${SDL2_IMAGE_LIB})
