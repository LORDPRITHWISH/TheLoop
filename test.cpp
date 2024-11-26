
#include <cmath>
#include <vector>
#include <iostream>

const int TERRAIN_SEGMENTS = 10;    // Number of segments visible on screen
const float TERRAIN_LENGTH = 100; // Length of visible terrain
const float TERRAIN_HEIGHT = 5.0f;  // Maximum height variation

std::vector<std::vector<float>> terrainPoints;

// Generate terrain points
void generateTerrain() {
    terrainPoints.clear();
    float segmentLength = TERRAIN_LENGTH / TERRAIN_SEGMENTS;
    for (int i = 0; i <= TERRAIN_SEGMENTS; ++i) {
        float x = i * segmentLength;
        float y = sinf(x * 0.5f) * TERRAIN_HEIGHT; // Example: sine wave
        terrainPoints.push_back({x, y});
    }
}

int main() {
    generateTerrain();
    for (auto& point : terrainPoints) {
        std::cout << "x: " << point[0] << ", y: " << point[1] << std::endl;
    }
    return 0;
}