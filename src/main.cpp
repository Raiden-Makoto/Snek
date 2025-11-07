#include "raylib.h"
#include <ctime>
#include <vector>
#include <string>
#include <queue>

using namespace std;

struct Position {
    int col;
    int row;
};

struct Direction {
    int dx;
    int dy;
};

int main() {
    const int boardSize = 720;
    const int scoreAreaHeight = 80;
    const int screenWidth = 720;
    const int screenHeight = boardSize + scoreAreaHeight; // 800 total

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Snake Game");

    // Set target FPS
    SetTargetFPS(60);

    const int cellSize = 30;
    const int boardStartY = scoreAreaHeight; // Board starts below score area
    const int totalGridWidth = boardSize / cellSize;
    const int totalGridHeight = boardSize / cellSize;
    // Playable area is reduced by 2 (1 border cell on each side)
    const int gridWidth = totalGridWidth - 2;
    const int gridHeight = totalGridHeight - 2;
    const int borderOffset = 1; // Border starts at cell 0, playable area starts at cell 1
    const Color grayColor = {18, 18, 18, 255}; // Very dark gray
    const Color snakeColor = {50, 150, 50, 255}; // Green for snake
    
    // Initialize random seed
    SetRandomSeed((unsigned int)time(nullptr));
    
    // Score
    int score = 0;
    
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
    
    // Queue for pending direction changes
    queue<Direction> directionQueue;
    
    // Movement timer (move every 0.25 seconds)
    float moveTimer = 0.0f;
    const float moveInterval = 0.25f;
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
                            // Check if snake ate food
                            bool ateFood = (newHead.col == foodCol && newHead.row == foodRow);
                            
                            // Move snake (add new head)
                            snake.insert(snake.begin(), newHead);
                            
                            if (ateFood) {
                                // Increase score
                                score++;
                                
                                // Snake grows by 2 units: 
                                // 1. Don't remove tail (already grows by 1 from adding new head)
                                // 2. Add an extra segment at the tail to grow by 2 total
                                snake.push_back(snake.back());
                                
                                // Spawn new food in a location not covered by snake
                                bool validPosition = false;
                                do {
                                    foodCol = GetRandomValue(0, gridWidth - 1);
                                    foodRow = GetRandomValue(0, gridHeight - 1);
                                    
                                    // Check if food position is not on any snake segment
                                    validPosition = true;
                                    for (const auto& segment : snake) {
                                        if (foodCol == segment.col && foodRow == segment.row) {
                                            validPosition = false;
                                            break;
                                        }
                                    }
                                } while (!validPosition);
                            } else {
                                // Remove tail (snake didn't grow)
                                snake.pop_back();
                            }
                        }
                    }
                }
            }
        }
        
        BeginDrawing();
        
        // Clear screen
        ClearBackground(BLACK);
        
        // Draw score area background
        DrawRectangle(0, 0, screenWidth, scoreAreaHeight, BLACK);
        
        // Draw score text
        const int fontSize = 40;
        string scoreText = "Score: " + to_string(score);
        int textWidth = MeasureText(scoreText.c_str(), fontSize);
        int textX = (screenWidth - textWidth) / 2;
        int textY = (scoreAreaHeight - fontSize) / 2;
        DrawText(scoreText.c_str(), textX, textY, fontSize, WHITE);
        
        // Draw white border cells (1 cell thick on all sides) - offset by score area
        // Top border
        for (int col = 0; col < totalGridWidth; col++) {
            DrawRectangle(col * cellSize, boardStartY, cellSize, cellSize, WHITE);
        }
        // Bottom border
        for (int col = 0; col < totalGridWidth; col++) {
            DrawRectangle(col * cellSize, boardStartY + (totalGridHeight - 1) * cellSize, cellSize, cellSize, WHITE);
        }
        // Left border
        for (int row = 1; row < totalGridHeight - 1; row++) {
            DrawRectangle(0, boardStartY + row * cellSize, cellSize, cellSize, WHITE);
        }
        // Right border
        for (int row = 1; row < totalGridHeight - 1; row++) {
            DrawRectangle((totalGridWidth - 1) * cellSize, boardStartY + row * cellSize, cellSize, cellSize, WHITE);
        }
        
        // Draw checkerboard pattern (only in playable area, offset by border and score area)
        for (int row = 0; row < gridHeight; row++) {
            for (int col = 0; col < gridWidth; col++) {
                int screenCol = (col + borderOffset) * cellSize;
                int screenRow = boardStartY + (row + borderOffset) * cellSize;
                // Alternate colors based on position
                if ((row + col) % 2 == 0) {
                    DrawRectangle(screenCol, screenRow, cellSize, cellSize, BLACK);
                } else {
                    DrawRectangle(screenCol, screenRow, cellSize, cellSize, grayColor);
                }
            }
        }
        
        // Draw food (red square) - offset by border and score area
        DrawRectangle((foodCol + borderOffset) * cellSize, boardStartY + (foodRow + borderOffset) * cellSize, cellSize, cellSize, RED);
        
        // Draw snake (green squares) - offset by border and score area
        for (const auto& segment : snake) {
            DrawRectangle((segment.col + borderOffset) * cellSize, boardStartY + (segment.row + borderOffset) * cellSize, cellSize, cellSize, snakeColor);
        }
        
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}

