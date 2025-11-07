#include "raylib.h"
#include <ctime>
#include <vector>
#include <string>
#include <deque>

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
    const Color snakeColor = {50, 150, 50, 255}; // Green for snake body
    const Color snakeHeadColor = {25, 100, 25, 255}; // Darker green for snake head
    const Color poisonColor = {181, 126, 107, 255}; // Light brown for poisonous apple
    const Color goldColor = {255, 165, 0, 255}; // Orange for gold apple
    const Color enchantedGoldColor = {255, 255, 0, 255}; // Yellow for enchanted gold apple
    const Color purpleColor = {186, 85, 211, 255}; // Purple for teleport apple
    
    enum FoodType { REGULAR, POISONOUS, GOLDEN, ENCHANTED_GOLDEN, TELEPORT };
    
    // Initialize random seed
    SetRandomSeed((unsigned int)time(nullptr));
    
    // Score
    int score = 0;
    int highScore = 0; // Session high score
    
    // Snake starts as a single segment (in playable area, offset by border)
    vector<Position> snake;
    snake.push_back({GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1)});
    
    // Food position (in playable area)
    int foodCol, foodRow;
    FoodType foodType = REGULAR;
    
    // Self-intersection immunity timer (for gold apple effect)
    float immunityTimer = 0.0f;
    const float immunityDuration = 10.0f;
    bool canIntersectSelf = false;
    
    // Wall-wrapping immunity timer (for enchanted gold apple effect)
    float wallImmunityTimer = 0.0f;
    const float wallImmunityDuration = 10.0f;
    bool canPassWalls = false;
    
    // Cannot eat apples debuff timer (for poisonous apple effect)
    float cannotEatTimer = 0.0f;
    const float cannotEatDuration = 10.0f;
    bool cannotEatApples = false;
    
    // Movement pause timer (after eating poisonous apple)
    float pauseTimer = 0.0f;
    const float pauseDuration = 0.5f;
    bool isPaused = false;
    
    // User pause system (P key toggle)
    bool isUserPaused = false;
    float resumeDelayTimer = 0.0f;
    const float resumeDelayDuration = 2.0f;
    bool isResuming = false;
    
    do {
        foodCol = GetRandomValue(0, gridWidth - 1);
        foodRow = GetRandomValue(0, gridHeight - 1);
    } while (foodCol == snake[0].col && foodRow == snake[0].row);
    
    // Determine food type: 4% regular gold, 1% enchanted gold, 10% poison, 3% purple, 82% regular
    int foodRoll = GetRandomValue(1, 100);
    if (foodRoll <= 4) {
        foodType = GOLDEN;
    } else if (foodRoll <= 5) {
        foodType = ENCHANTED_GOLDEN;
    } else if (foodRoll <= 15) {
        foodType = POISONOUS;
    } else if (foodRoll <= 18) {
        foodType = TELEPORT;
    } else {
        foodType = REGULAR;
    }
    
    // Direction: starts at (0, 0) - not moving
    int dx = 0;
    int dy = 0;
    
    // Deque for pending direction changes (allows access to front and back)
    deque<Direction> directionQueue;
    
    // Movement timer (move every 0.25 seconds)
    float moveTimer = 0.0f;
    const float moveInterval = 0.25f;
    bool gameOver = false;
    bool showInstructions = true; // Show instructions screen at start

    // Main game loop
    while (!WindowShouldClose()) {
        // Handle instructions screen
        if (showInstructions) {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                showInstructions = false;
            }
            
            BeginDrawing();
            ClearBackground(BLACK);
            
            // Title
            const int titleFontSize = 50;
            string titleText = "SNAKE GAME";
            int titleWidth = MeasureText(titleText.c_str(), titleFontSize);
            int titleX = (screenWidth - titleWidth) / 2;
            int titleY = 40;
            DrawText(titleText.c_str(), titleX, titleY, titleFontSize, WHITE);
            
            // Instructions header
            const int headerFontSize = 32;
            string headerText = "APPLE TYPES";
            int headerWidth = MeasureText(headerText.c_str(), headerFontSize);
            int headerX = (screenWidth - headerWidth) / 2;
            int headerY = titleY + 70;
            DrawText(headerText.c_str(), headerX, headerY, headerFontSize, YELLOW);
            
            // Apple type instructions
            const int textFontSize = 20;
            int startY = headerY + 50;
            int lineHeight = 28;
            int leftMargin = 40;
            int rightMargin = 40;
            int currentY = startY;
            
            // Regular Apple
            DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, RED);
            string regularText = "Regular Apple (Red) - 82%: Score +1, Grow +2 units";
            DrawText(regularText.c_str(), leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight;
            
            // Poisonous Apple
            DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, poisonColor);
            string poisonText = "Poisonous Apple (Brown) - 10%: Reverses direction, 10s debuff";
            DrawText(poisonText.c_str(), leftMargin, currentY, textFontSize, WHITE);
            string poisonSubText = "  Cannot eat regular/purple apples during debuff";
            DrawText(poisonSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
            currentY += lineHeight * 2;
            
            // Gold Apple
            DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, goldColor);
            string goldText = "Gold Apple (Orange) - 4%: Score +2, Resistance 10s";
            DrawText(goldText.c_str(), leftMargin, currentY, textFontSize, WHITE);
            string goldSubText = "  Can pass through own body, works when poisoned";
            DrawText(goldSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
            currentY += lineHeight * 2;
            
            // Enchanted Gold Apple
            DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, enchantedGoldColor);
            string enchantedText = "Enchanted Gold (Yellow) - 1%: Score +2, Resistance II 10s";
            DrawText(enchantedText.c_str(), leftMargin, currentY, textFontSize, WHITE);
            string enchantedSubText = "  Pass through body + walls, works when poisoned";
            DrawText(enchantedSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
            currentY += lineHeight * 2;
            
            // Purple Apple
            DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, purpleColor);
            string purpleText = "Purple Apple (Purple) - 3%: Teleport to random location";
            DrawText(purpleText.c_str(), leftMargin, currentY, textFontSize, WHITE);
            string purpleSubText = "  No growth, cannot be eaten when poisoned";
            DrawText(purpleSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
            currentY += lineHeight * 2 + 20;
            
            // Controls header
            string controlsHeader = "CONTROLS";
            int controlsHeaderWidth = MeasureText(controlsHeader.c_str(), headerFontSize);
            int controlsHeaderX = (screenWidth - controlsHeaderWidth) / 2;
            DrawText(controlsHeader.c_str(), controlsHeaderX, currentY, headerFontSize, YELLOW);
            currentY += lineHeight + 10;
            
            // Controls
            DrawText("Arrow Keys / WASD - Move", leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight;
            DrawText("P - Pause/Unpause", leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight;
            DrawText("Q - Show game over screen (or quit)", leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight;
            DrawText("ESC - Exit game", leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight;
            DrawText("R / Space - Restart (on game over)", leftMargin, currentY, textFontSize, WHITE);
            currentY += lineHeight * 2;
            
            // Start prompt
            string startText = "Press SPACE or ENTER to start";
            int startTextWidth = MeasureText(startText.c_str(), textFontSize + 4);
            int startTextX = (screenWidth - startTextWidth) / 2;
            DrawText(startText.c_str(), startTextX, currentY, textFontSize + 4, GREEN);
            
            EndDrawing();
            continue; // Skip game logic while showing instructions
        }
        float deltaTime = GetFrameTime();
        
        // Update immunity timer (only when not user paused or resuming)
        if (!isUserPaused && !isResuming) {
            if (canIntersectSelf) {
                immunityTimer -= deltaTime;
                if (immunityTimer <= 0.0f) {
                    canIntersectSelf = false;
                    immunityTimer = 0.0f;
                }
            }
            
            // Update cannot eat apples debuff timer
            if (cannotEatApples) {
                cannotEatTimer -= deltaTime;
                if (cannotEatTimer <= 0.0f) {
                    cannotEatApples = false;
                    cannotEatTimer = 0.0f;
                }
            }
            
            // Update wall-wrapping immunity timer
            if (canPassWalls) {
                wallImmunityTimer -= deltaTime;
                if (wallImmunityTimer <= 0.0f) {
                    canPassWalls = false;
                    wallImmunityTimer = 0.0f;
                }
            }
        }
        
        // Update movement pause timer (for poisonous apple)
        if (isPaused) {
            pauseTimer -= deltaTime;
            if (pauseTimer <= 0.0f) {
                isPaused = false;
                pauseTimer = 0.0f;
            }
        }
        
        // Update resume delay timer (after unpausing)
        if (isResuming) {
            resumeDelayTimer -= deltaTime;
            if (resumeDelayTimer <= 0.0f) {
                isResuming = false;
                resumeDelayTimer = 0.0f;
            }
        }
        
        // Handle quit input
        if (IsKeyPressed(KEY_ESCAPE)) {
            break; // ESC always exits immediately
        }
        
        // Q key: show game over screen if not game over, otherwise exit
        if (IsKeyPressed(KEY_Q)) {
            if (!gameOver) {
                gameOver = true; // Show game over screen
            } else {
                break; // Exit if already on game over screen
            }
        }
        
        // Handle user pause toggle (P key) - available when game is not over
        if (!gameOver && IsKeyPressed(KEY_P)) {
            if (isUserPaused) {
                // Unpause - start resume delay
                isResuming = true;
                resumeDelayTimer = resumeDelayDuration;
                isUserPaused = false;
            } else if (!isResuming) {
                // Pause
                isUserPaused = true;
            }
        }
        
        // Handle game over screen input
        if (gameOver) {
            if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_SPACE)) {
                // Restart game - reset all state
                gameOver = false;
                score = 0;
                showInstructions = false; // Don't show instructions on restart
                
                // Reset snake
                snake.clear();
                snake.push_back({GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1)});
                
                // Reset food
                do {
                    foodCol = GetRandomValue(0, gridWidth - 1);
                    foodRow = GetRandomValue(0, gridHeight - 1);
                } while (foodCol == snake[0].col && foodRow == snake[0].row);
                
                // Reset food type
                int foodRoll = GetRandomValue(1, 100);
                if (foodRoll <= 4) {
                    foodType = GOLDEN;
                } else if (foodRoll <= 5) {
                    foodType = ENCHANTED_GOLDEN;
                } else if (foodRoll <= 15) {
                    foodType = POISONOUS;
                } else if (foodRoll <= 18) {
                    foodType = TELEPORT;
                } else {
                    foodType = REGULAR;
                }
                
                // Reset direction and movement
                dx = 0;
                dy = 0;
                directionQueue.clear();
                moveTimer = 0.0f;
                
                // Reset all timers and effects
                canIntersectSelf = false;
                immunityTimer = 0.0f;
                canPassWalls = false;
                wallImmunityTimer = 0.0f;
                cannotEatApples = false;
                cannotEatTimer = 0.0f;
                isPaused = false;
                pauseTimer = 0.0f;
                isUserPaused = false;
                isResuming = false;
                resumeDelayTimer = 0.0f;
            }
        }
        
        if (!gameOver && !isUserPaused && !isResuming) {
            // Handle input for direction change - add to deque
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                Direction newDir = {0, -1};
                // Only add if not reversing current direction and not same as last queued
                if (((dx == 0 && dy == 0) || dy != 1) && 
                    (directionQueue.empty() || 
                     directionQueue.back().dx != newDir.dx || 
                     directionQueue.back().dy != newDir.dy)) {
                    directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                Direction newDir = {0, 1};
                if (((dx == 0 && dy == 0) || dy != -1) && 
                    (directionQueue.empty() || 
                     directionQueue.back().dx != newDir.dx || 
                     directionQueue.back().dy != newDir.dy)) {
                    directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                Direction newDir = {-1, 0};
                if (((dx == 0 && dy == 0) || dx != 1) && 
                    (directionQueue.empty() || 
                     directionQueue.back().dx != newDir.dx || 
                     directionQueue.back().dy != newDir.dy)) {
                    directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                Direction newDir = {1, 0};
                if (((dx == 0 && dy == 0) || dx != -1) && 
                    (directionQueue.empty() || 
                     directionQueue.back().dx != newDir.dx || 
                     directionQueue.back().dy != newDir.dy)) {
                    directionQueue.push_back(newDir);
                }
            }
            
            // Update movement timer (always runs, even when stationary, to allow starting)
            // Skip timer update if paused (poisonous apple pause) or user paused or resuming
            if (!isPaused && !isUserPaused && !isResuming) {
                moveTimer += deltaTime;
            }
            
            // Process movement when timer elapses (skip if paused or resuming)
            if (!isPaused && !isUserPaused && !isResuming && moveTimer >= moveInterval) {
                moveTimer = 0.0f;
                
                // Process direction queue (get next direction if available)
                if (!directionQueue.empty()) {
                    Direction nextDir = directionQueue.front();
                    // Only apply if not reversing current direction (or snake is stationary)
                    if ((dx == 0 && dy == 0) || (nextDir.dx != -dx || nextDir.dy != -dy)) {
                        dx = nextDir.dx;
                        dy = nextDir.dy;
                    }
                    directionQueue.pop_front();
                }
                
                // Move if we have a direction
                if (dx != 0 || dy != 0) {
                    // Calculate new head position
                    Position newHead = {snake[0].col + dx, snake[0].row + dy};
                    
                    // Check wall collision (wrap around if canPassWalls is enabled)
                    if (canPassWalls) {
                        // Wrap around the edges
                        if (newHead.col < 0) {
                            newHead.col = gridWidth - 1;
                        } else if (newHead.col >= gridWidth) {
                            newHead.col = 0;
                        }
                        if (newHead.row < 0) {
                            newHead.row = gridHeight - 1;
                        } else if (newHead.row >= gridHeight) {
                            newHead.row = 0;
                        }
                    } else {
                        // Normal wall collision
                        if (newHead.col < 0 || newHead.col >= gridWidth || 
                            newHead.row < 0 || newHead.row >= gridHeight) {
                            // Update high score before game over
                            if (score > highScore) {
                                highScore = score;
                            }
                            gameOver = true;
                        }
                    }
                    
                    if (!gameOver) {
                        // Check self collision (only if not immune)
                        bool hitSelf = false;
                        if (!canIntersectSelf) {
                            for (const auto& segment : snake) {
                                if (newHead.col == segment.col && newHead.row == segment.row) {
                                    hitSelf = true;
                                    break;
                                }
                            }
                        }
                        
                        if (hitSelf) {
                            // Add the head position so it's drawn over the body part it collided with
                            snake.insert(snake.begin(), newHead);
                            // Update high score before game over
                            if (score > highScore) {
                                highScore = score;
                            }
                            gameOver = true;
                        } else {
                            // Check if snake ate food
                            bool ateFood = (newHead.col == foodCol && newHead.row == foodRow);
                            
                            // Move snake (add new head)
                            snake.insert(snake.begin(), newHead);
                            
                            if (ateFood) {
                                if (foodType == POISONOUS) {
                                    // Poisonous apple - pause movement for 0.5 seconds before reversing
                                    isPaused = true;
                                    pauseTimer = pauseDuration;
                                    
                                    // Store the reverse direction (will be applied after pause)
                                    // We'll reverse direction after the pause, so store current direction
                                    // Clear direction queue to avoid confusion
                                    directionQueue.clear();

                                    // Reverse the snake body (head becomes tail, tail becomes head)
                                    reverse(snake.begin(), snake.end());

                                    // Remove tail since we reversed (we just added a new head, need to remove old tail)
                                    snake.pop_back();

                                    // Reverse direction (but movement is paused)
                                    dx = -dx;
                                    dy = -dy;
                                    
                                    // Enable cannot eat apples debuff for 5 seconds
                                    cannotEatApples = true;
                                    cannotEatTimer = cannotEatDuration;
                                } else if (foodType == TELEPORT) {
                                    // Purple apple - only works if not debuffed (same as regular apples)
                                    if (!cannotEatApples) {
                                        // Purple apple - teleport snake to random location with random orientation
                                        // Remove the head we just added (since we're teleporting)
                                        snake.erase(snake.begin());
                                        
                                        // Save current snake length (before removing head)
                                        int snakeLength = snake.size();
                                        
                                        // Find a random valid position for the snake head (not on food)
                                        int newHeadCol, newHeadRow;
                                        bool validTeleportPos = false;
                                        do {
                                            newHeadCol = GetRandomValue(0, gridWidth - 1);
                                            newHeadRow = GetRandomValue(0, gridHeight - 1);
                                            
                                            // Check if position is not on food
                                            validTeleportPos = (newHeadCol != foodCol || newHeadRow != foodRow);
                                        } while (!validTeleportPos);
                                        
                                        // Choose a random direction
                                        int dirRoll = GetRandomValue(0, 3);
                                        switch (dirRoll) {
                                            case 0: dx = 0; dy = -1; break; // UP
                                            case 1: dx = 0; dy = 1; break;  // DOWN
                                            case 2: dx = -1; dy = 0; break; // LEFT
                                            case 3: dx = 1; dy = 0; break;  // RIGHT
                                        }
                                        
                                        // Rebuild snake at new position with new orientation (same length)
                                        snake.clear();
                                        snake.push_back({newHeadCol, newHeadRow});
                                        
                                        // Add body segments behind head in opposite direction of movement
                                        for (int i = 1; i < snakeLength; i++) {
                                            int segCol = newHeadCol - dx * i;
                                            int segRow = newHeadRow - dy * i;
                                            
                                            // Keep segments within bounds (clamp to edges)
                                            if (segCol < 0) segCol = 0;
                                            if (segCol >= gridWidth) segCol = gridWidth - 1;
                                            if (segRow < 0) segRow = 0;
                                            if (segRow >= gridHeight) segRow = gridHeight - 1;
                                            
                                            snake.push_back({segCol, segRow});
                                        }
                                        
                                        // Clear direction queue and stop movement (wait for user input)
                                        directionQueue.clear();
                                        dx = 0;
                                        dy = 0;
                                        moveTimer = 0.0f; // Reset movement timer
                                    } else {
                                        // If debuffed, purple apple is consumed but doesn't teleport
                                        // Remove the tail to keep snake length the same (no growth)
                                        snake.pop_back();
                                    }
                                } else if (foodType == GOLDEN || foodType == ENCHANTED_GOLDEN) {
                                    // Gold apple (regular or enchanted) - always works, even with debuff
                                    // Increase score by 2, grow, and enable self-intersection
                                    score += 2;
                                    if (score > highScore) {
                                        highScore = score;
                                    }
                                    
                                    // Snake grows by 2 units
                                    snake.push_back(snake.back());
                                    
                                    // Enable self-intersection for 5 seconds
                                    canIntersectSelf = true;
                                    immunityTimer = immunityDuration;
                                    
                                    // Enchanted gold apples also enable wall-wrapping
                                    if (foodType == ENCHANTED_GOLDEN) {
                                        canPassWalls = true;
                                        wallImmunityTimer = wallImmunityDuration;
                                    }
                                } else {
                                    // Regular apple - only works if not debuffed
                                    if (!cannotEatApples) {
                                        // Increase score and grow
                                        score++;
                                        if (score > highScore) {
                                            highScore = score;
                                        }
                                        
                                        // Snake grows by 2 units: 
                                        // 1. Don't remove tail (already grows by 1 from adding new head)
                                        // 2. Add an extra segment at the tail to grow by 2 total
                                        snake.push_back(snake.back());
                                    } else {
                                        // If debuffed, apple is consumed but doesn't give benefits
                                        // Remove the tail to keep snake length the same
                                        snake.pop_back();
                                    }
                                }
                                
                                // Spawn new food in a location not covered by snake (always spawn new food when any apple is eaten)
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
                                
                                // Determine food type: 4% regular gold, 1% enchanted gold, 10% poison, 3% purple, 82% regular
                                int foodRoll = GetRandomValue(1, 100);
                                if (foodRoll <= 4) {
                                    foodType = GOLDEN;
                                } else if (foodRoll <= 5) {
                                    foodType = ENCHANTED_GOLDEN;
                                } else if (foodRoll <= 15) {
                                    foodType = POISONOUS;
                                } else if (foodRoll <= 18) {
                                    foodType = TELEPORT;
                                } else {
                                    foodType = REGULAR;
                                }
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
        
        // Draw high score text (smaller, to the right)
        const int highScoreFontSize = 24;
        string highScoreText = "High: " + to_string(highScore);
        int highScoreX = screenWidth - MeasureText(highScoreText.c_str(), highScoreFontSize) - 20;
        int highScoreY = (scoreAreaHeight - highScoreFontSize) / 2;
        DrawText(highScoreText.c_str(), highScoreX, highScoreY, highScoreFontSize, LIGHTGRAY);
        
        // Draw status effect countdowns (top right, below high score)
        const int statusFontSize = 18;
        int statusY = highScoreY + highScoreFontSize + 5;
        int statusRightMargin = 20;
        Color statusColor;
        
        // Draw poisoned status (if active)
        if (cannotEatApples && cannotEatTimer > 0.0f) {
            int countdown = (int)ceil(cannotEatTimer);
            string statusText = "Poisoned: " + to_string(countdown);
            int statusX = screenWidth - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
            statusColor = {181, 126, 107, 255}; // Light brown (same as poison apple)
            DrawText(statusText.c_str(), statusX, statusY, statusFontSize, statusColor);
            statusY += statusFontSize + 3;
        }
        
        // Draw Resistance status (self-intersection immunity from gold apple)
        if (canIntersectSelf && immunityTimer > 0.0f) {
            int countdown = (int)ceil(immunityTimer);
            string statusText = "Resistance: " + to_string(countdown);
            int statusX = screenWidth - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
            statusColor = {255, 165, 0, 255}; // Orange (same as gold apple)
            DrawText(statusText.c_str(), statusX, statusY, statusFontSize, statusColor);
            statusY += statusFontSize + 3;
        }
        
        // Draw Resistance II status (wall-wrapping from enchanted gold apple)
        if (canPassWalls && wallImmunityTimer > 0.0f) {
            int countdown = (int)ceil(wallImmunityTimer);
            string statusText = "Resistance II: " + to_string(countdown);
            int statusX = screenWidth - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
            statusColor = {255, 255, 0, 255}; // Yellow (same as enchanted gold apple)
            DrawText(statusText.c_str(), statusX, statusY, statusFontSize, statusColor);
        }
        
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
        
        // Draw food (red for normal, light brown for poisonous, orange-yellow for gold, brighter yellow for enchanted gold, purple for teleport) - offset by border and score area
        Color foodColor;
        if (foodType == ENCHANTED_GOLDEN) {
            foodColor = enchantedGoldColor;
        } else if (foodType == GOLDEN) {
            foodColor = goldColor;
        } else if (foodType == POISONOUS) {
            foodColor = poisonColor;
        } else if (foodType == TELEPORT) {
            foodColor = purpleColor;
        } else {
            foodColor = RED;
        }
        DrawRectangle((foodCol + borderOffset) * cellSize, boardStartY + (foodRow + borderOffset) * cellSize, cellSize, cellSize, foodColor);
        
        // Draw snake body first (regular green) - offset by border and score area
        for (size_t i = 1; i < snake.size(); i++) {
            const auto& segment = snake[i];
            DrawRectangle((segment.col + borderOffset) * cellSize, boardStartY + (segment.row + borderOffset) * cellSize, cellSize, cellSize, snakeColor);
        }
        
        // Draw snake head last (darker green) so it appears on top - offset by border and score area
        if (!snake.empty()) {
            const auto& head = snake[0];
            DrawRectangle((head.col + borderOffset) * cellSize, boardStartY + (head.row + borderOffset) * cellSize, cellSize, cellSize, snakeHeadColor);
        }
        
        // Draw pause screen overlay
        if (isUserPaused && !gameOver) {
            // Semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});
            
            // Paused text
            const int pauseFontSize = 60;
            string pauseText = "PAUSED";
            int pauseTextWidth = MeasureText(pauseText.c_str(), pauseFontSize);
            int pauseX = (screenWidth - pauseTextWidth) / 2;
            int pauseY = screenHeight / 2 - 30;
            DrawText(pauseText.c_str(), pauseX, pauseY, pauseFontSize, WHITE);
            
            // Instructions
            const int instructionFontSize = 24;
            string resumeText = "Press P to resume (or Q to quit)";
            int resumeTextWidth = MeasureText(resumeText.c_str(), instructionFontSize);
            int resumeX = (screenWidth - resumeTextWidth) / 2;
            DrawText(resumeText.c_str(), resumeX, pauseY + 80, instructionFontSize, LIGHTGRAY);
        }
        
        // Draw resuming countdown overlay
        if (isResuming && !gameOver) {
            // Semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});
            
            // Resuming text with countdown
            const int resumeFontSize = 40;
            int countdown = (int)ceil(resumeDelayTimer);
            string resumeText = "Resuming in " + to_string(countdown) + "...";
            int resumeTextWidth = MeasureText(resumeText.c_str(), resumeFontSize);
            int resumeX = (screenWidth - resumeTextWidth) / 2;
            int resumeY = screenHeight / 2;
            DrawText(resumeText.c_str(), resumeX, resumeY, resumeFontSize, WHITE);
        }
        
        // Draw game over screen overlay
        if (gameOver) {
            // Semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});
            
            // Game Over text
            const int gameOverFontSize = 60;
            string gameOverText = "GAME OVER";
            int gameOverTextWidth = MeasureText(gameOverText.c_str(), gameOverFontSize);
            int gameOverX = (screenWidth - gameOverTextWidth) / 2;
            int gameOverY = screenHeight / 2 - 100;
            DrawText(gameOverText.c_str(), gameOverX, gameOverY, gameOverFontSize, WHITE);
            
            // Final score
            const int finalScoreFontSize = 40;
            string finalScoreText = "Final Score: " + to_string(score);
            int finalScoreTextWidth = MeasureText(finalScoreText.c_str(), finalScoreFontSize);
            int finalScoreX = (screenWidth - finalScoreTextWidth) / 2;
            int finalScoreY = gameOverY + 80;
            DrawText(finalScoreText.c_str(), finalScoreX, finalScoreY, finalScoreFontSize, WHITE);
            
            // High score (same font size as final score)
            const int highScoreFontSize = 40;
            string highScoreText = "High Score: " + to_string(highScore);
            int highScoreTextWidth = MeasureText(highScoreText.c_str(), highScoreFontSize);
            int highScoreX = (screenWidth - highScoreTextWidth) / 2;
            int highScoreY = finalScoreY + 60;
            DrawText(highScoreText.c_str(), highScoreX, highScoreY, highScoreFontSize, YELLOW);
            
            // Instructions
            const int instructionFontSize = 24;
            string restartText = "Press R or SPACE to restart";
            string quitText = "Press ESC to exit or Q to quit";
            int restartTextWidth = MeasureText(restartText.c_str(), instructionFontSize);
            int quitTextWidth = MeasureText(quitText.c_str(), instructionFontSize);
            int restartX = (screenWidth - restartTextWidth) / 2;
            int quitX = (screenWidth - quitTextWidth) / 2;
            int instructionY = highScoreY + 80; // More spacing after high score
            DrawText(restartText.c_str(), restartX, instructionY, instructionFontSize, LIGHTGRAY);
            DrawText(quitText.c_str(), quitX, instructionY + 35, instructionFontSize, LIGHTGRAY);
        }
        
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}

