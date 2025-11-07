#include "raylib.h"

int main() {
    const int screenWidth = 720;
    const int screenHeight = 720;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Snake Game");

    // Set target FPS
    SetTargetFPS(60);

    const int cellSize = 30;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;
    const Color grayColor = {18, 18, 18, 255}; // Very dark gray

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        
        // Clear screen
        ClearBackground(BLACK);
        
        // Draw checkerboard pattern
        for (int row = 0; row < gridHeight; row++) {
            for (int col = 0; col < gridWidth; col++) {
                // Alternate colors based on position
                if ((row + col) % 2 == 0) {
                    DrawRectangle(col * cellSize, row * cellSize, cellSize, cellSize, BLACK);
                } else {
                    DrawRectangle(col * cellSize, row * cellSize, cellSize, cellSize, grayColor);
                }
            }
        }
        
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}

