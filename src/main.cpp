#include "raylib.h"
#include <ctime>
#include <vector>

using namespace std;

struct Position {
    int col;
    int row;
};

int main() {
    const int screenWidth = 720;
    const int screenHeight = 720;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Snake Game");

    // Set target FPS
    SetTargetFPS(60);

    const int cellSize = 30;
    const int totalGridWidth = screenWidth / cellSize;
    const int totalGridHeight = screenHeight / cellSize;
    // Playable area is reduced by 2 (1 border cell on each side)
    const int gridWidth = totalGridWidth - 2;
    const int gridHeight = totalGridHeight - 2;
    const int borderOffset = 1; // Border starts at cell 0, playable area starts at cell 1
    const Color grayColor = {18, 18, 18, 255}; // Very dark gray
    const Color snakeColor = {50, 150, 50, 255}; // Green for snake
    
    // Initialize random seed
    SetRandomSeed((unsigned int)time(nullptr));
    
    // Snake starts as a single segment (in playable area, offset by border)
    vector<Position> snake;
    snake.push_back({GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1)});
    
    // Food position (in playable area)
    int foodCol, foodRow;
    do {
        foodCol = GetRandomValue(0, gridWidth - 1);
        foodRow = GetRandomValue(0, gridHeight - 1);
    } while (foodCol == snake[0].col && foodRow == snake[0].row);
    
    // Direction: starts at (0, 0) - not moving
    int dx = 0;
    int dy = 0;
    
    // Movement timer (move every 0.5 seconds)
    float moveTimer = 0.0f;
    const float moveInterval = 0.5f;
    bool gameOver = false;

    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        if (!gameOver) {
            // Handle input for direction change (only when key is pressed)
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                if (dy == 0) { // Prevent reversing into itself
                    dx = 0;
                    dy = -1;
                }
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                if (dy == 0) {
                    dx = 0;
                    dy = 1;
                }
            }
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                if (dx == 0) {
                    dx = -1;
                    dy = 0;
                }
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                if (dx == 0) {
                    dx = 1;
                    dy = 0;
                }
            }
            
            // Move snake automatically at intervals
            if (dx != 0 || dy != 0) {
                moveTimer += deltaTime;
                if (moveTimer >= moveInterval) {
                    moveTimer = 0.0f;
                    
                    // Calculate new head position
                    Position newHead = {snake[0].col + dx, snake[0].row + dy};
                    
                    // Check wall collision
                    if (newHead.col < 0 || newHead.col >= gridWidth || 
                        newHead.row < 0 || newHead.row >= gridHeight) {
                        gameOver = true;
                    } else {
                        // Check self collision
                        bool hitSelf = false;
                        for (const auto& segment : snake) {
                            if (newHead.col == segment.col && newHead.row == segment.row) {
                                hitSelf = true;
                                break;
                            }
                        }
                        
                        if (hitSelf) {
                            gameOver = true;
                        } else {
                            // Move snake (add new head)
                            snake.insert(snake.begin(), newHead);
                            // For now, remove tail (we'll add food eating later)
                            snake.pop_back();
                        }
                    }
                }
            }
        }
        
        BeginDrawing();
        
        // Clear screen
        ClearBackground(BLACK);
        
        // Draw white border cells (1 cell thick on all sides)
        // Top border
        for (int col = 0; col < totalGridWidth; col++) {
            DrawRectangle(col * cellSize, 0, cellSize, cellSize, WHITE);
        }
        // Bottom border
        for (int col = 0; col < totalGridWidth; col++) {
            DrawRectangle(col * cellSize, (totalGridHeight - 1) * cellSize, cellSize, cellSize, WHITE);
        }
        // Left border
        for (int row = 1; row < totalGridHeight - 1; row++) {
            DrawRectangle(0, row * cellSize, cellSize, cellSize, WHITE);
        }
        // Right border
        for (int row = 1; row < totalGridHeight - 1; row++) {
            DrawRectangle((totalGridWidth - 1) * cellSize, row * cellSize, cellSize, cellSize, WHITE);
        }
        
        // Draw checkerboard pattern (only in playable area, offset by border)
        for (int row = 0; row < gridHeight; row++) {
            for (int col = 0; col < gridWidth; col++) {
                int screenCol = (col + borderOffset) * cellSize;
                int screenRow = (row + borderOffset) * cellSize;
                // Alternate colors based on position
                if ((row + col) % 2 == 0) {
                    DrawRectangle(screenCol, screenRow, cellSize, cellSize, BLACK);
                } else {
                    DrawRectangle(screenCol, screenRow, cellSize, cellSize, grayColor);
                }
            }
        }
        
        // Draw food (red square) - offset by border
        DrawRectangle((foodCol + borderOffset) * cellSize, (foodRow + borderOffset) * cellSize, cellSize, cellSize, RED);
        
        // Draw snake (green squares) - offset by border
        for (const auto& segment : snake) {
            DrawRectangle((segment.col + borderOffset) * cellSize, (segment.row + borderOffset) * cellSize, cellSize, cellSize, snakeColor);
        }
        
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}

