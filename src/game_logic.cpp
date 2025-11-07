#include "game_logic.h"
#include "game_types.h"
#include "raylib.h"
#include <algorithm>

void GameLogic::ProcessMovement(GameState& state, float deltaTime) {
    if (state.gameOver || state.isUserPaused || state.isResuming) {
        return;
    }
    
    // Update movement timer
    if (!state.isPaused && !state.isUserPaused && !state.isResuming) {
        state.moveTimer += deltaTime;
    }
    
    // Get move interval based on game mode
    float moveInterval = (state.gameMode == MODE_ACCELERATED) 
        ? GameConstants::MOVE_INTERVAL_ACCELERATED 
        : GameConstants::MOVE_INTERVAL_REGULAR;
    
    // Process movement when timer elapses
    if (!state.isPaused && !state.isUserPaused && !state.isResuming && 
        state.moveTimer >= moveInterval) {
        state.moveTimer = 0.0f;
        
        // Process direction queue
        ProcessDirectionQueue(state);
        
        // Move if we have a direction
        if (state.dx != 0 || state.dy != 0) {
            Position newHead = {state.snake[0].col + state.dx, state.snake[0].row + state.dy};
            
            // Check wall collision
            if (state.canPassWalls) {
                // Wrap around
                if (newHead.col < 0) {
                    newHead.col = GameConstants::GRID_WIDTH - 1;
                } else if (newHead.col >= GameConstants::GRID_WIDTH) {
                    newHead.col = 0;
                }
                if (newHead.row < 0) {
                    newHead.row = GameConstants::GRID_HEIGHT - 1;
                } else if (newHead.row >= GameConstants::GRID_HEIGHT) {
                    newHead.row = 0;
                }
            } else {
                // Normal wall collision
                if (newHead.col < 0 || newHead.col >= GameConstants::GRID_WIDTH || 
                    newHead.row < 0 || newHead.row >= GameConstants::GRID_HEIGHT) {
                    state.UpdateHighScore();
                    state.gameOver = true;
                    if (!state.gameOverSoundPlayed) {
                        PlaySound(state.gameOverSound);
                        state.gameOverSoundPlayed = true;
                    }
                    return;
                }
            }
            
            if (!state.gameOver) {
                CheckCollisions(state, newHead);
            }
        }
    }
}

void GameLogic::ProcessDirectionQueue(GameState& state) {
    if (!state.directionQueue.empty()) {
        Direction nextDir = state.directionQueue.front();
        // Only apply if not reversing current direction (or snake is stationary)
        if ((state.dx == 0 && state.dy == 0) || 
            (nextDir.dx != -state.dx || nextDir.dy != -state.dy)) {
            state.dx = nextDir.dx;
            state.dy = nextDir.dy;
        }
        state.directionQueue.pop_front();
    }
}

void GameLogic::CheckCollisions(GameState& state, Position newHead) {
    // Check self collision
    bool hitSelf = false;
    if (!state.canIntersectSelf) {
        for (const auto& segment : state.snake) {
            if (newHead.col == segment.col && newHead.row == segment.row) {
                hitSelf = true;
                break;
            }
        }
    }
    
    if (hitSelf) {
        state.snake.insert(state.snake.begin(), newHead);
        state.UpdateHighScore();
        state.gameOver = true;
        if (!state.gameOverSoundPlayed) {
            PlaySound(state.gameOverSound);
            state.gameOverSoundPlayed = true;
        }
        return;
    }
    
    // Check if snake ate any apple
    int eatenAppleIndex = -1;
    FoodType eatenFoodType = REGULAR;
    for (size_t i = 0; i < state.apples.size(); i++) {
        if (newHead.col == state.apples[i].col && newHead.row == state.apples[i].row) {
            eatenAppleIndex = i;
            eatenFoodType = state.apples[i].type;
            break;
        }
    }
    
    // Move snake
    state.snake.insert(state.snake.begin(), newHead);
    
    if (eatenAppleIndex >= 0) {
        HandleAppleConsumption(state, eatenAppleIndex);
    } else {
        // Remove tail (snake didn't grow)
        state.snake.pop_back();
    }
}

void GameLogic::HandleAppleConsumption(GameState& state, int eatenAppleIndex) {
    // Remove the eaten apple
    FoodType eatenFoodType = state.apples[eatenAppleIndex].type;
    state.apples.erase(state.apples.begin() + eatenAppleIndex);
    
    if (eatenFoodType == POISONOUS) {
        // Poisonous apple - pause movement and reverse
        state.isPaused = true;
        state.pauseTimer = GameConstants::PAUSE_DURATION;
        state.directionQueue.clear();
        
        std::reverse(state.snake.begin(), state.snake.end());
        state.snake.pop_back();
        
        state.dx = -state.dx;
        state.dy = -state.dy;
        
        state.cannotEatApples = true;
        state.cannotEatTimer = GameConstants::CANNOT_EAT_DURATION;
        state.poisonSoundTimer = 1.0f;
    } else if (eatenFoodType == TELEPORT) {
        if (!state.cannotEatApples) {
            // Purple apple - teleport
            state.snake.erase(state.snake.begin());
            int snakeLength = state.snake.size();
            
            int newHeadCol, newHeadRow;
            bool validTeleportPos = false;
            do {
                newHeadCol = GetRandomValue(0, GameConstants::GRID_WIDTH - 1);
                newHeadRow = GetRandomValue(0, GameConstants::GRID_HEIGHT - 1);
                validTeleportPos = state.IsValidPosition(newHeadCol, newHeadRow);
            } while (!validTeleportPos);
            
            int dirRoll = GetRandomValue(0, 3);
            switch (dirRoll) {
                case 0: state.dx = 0; state.dy = -1; break;
                case 1: state.dx = 0; state.dy = 1; break;
                case 2: state.dx = -1; state.dy = 0; break;
                case 3: state.dx = 1; state.dy = 0; break;
            }
            
            state.snake.clear();
            state.snake.push_back({newHeadCol, newHeadRow});
            
            for (int i = 1; i < snakeLength; i++) {
                int segCol = newHeadCol - state.dx * i;
                int segRow = newHeadRow - state.dy * i;
                
                if (segCol < 0) segCol = 0;
                if (segCol >= GameConstants::GRID_WIDTH) segCol = GameConstants::GRID_WIDTH - 1;
                if (segRow < 0) segRow = 0;
                if (segRow >= GameConstants::GRID_HEIGHT) segRow = GameConstants::GRID_HEIGHT - 1;
                
                state.snake.push_back({segCol, segRow});
            }
            
            state.directionQueue.clear();
            state.dx = 0;
            state.dy = 0;
            state.moveTimer = 0.0f;
            
            PlaySound(state.purpleSound);
        } else {
            state.snake.pop_back();
        }
    } else if (eatenFoodType == POMME_PLUS || eatenFoodType == POMME_SUPREME) {
        // Pomme Plus or Pomme Supreme
        state.score += 2;
        state.UpdateHighScore();
        
        state.snake.push_back(state.snake.back());
        state.canIntersectSelf = true;
        state.immunityTimer = GameConstants::IMMUNITY_DURATION;
        
        if (eatenFoodType == POMME_SUPREME) {
            state.canPassWalls = true;
            state.wallImmunityTimer = GameConstants::WALL_IMMUNITY_DURATION;
        }
        
        PlaySound(state.goldenSound);
    } else {
        // Regular apple
        if (!state.cannotEatApples) {
            state.score++;
            state.UpdateHighScore();
            state.snake.push_back(state.snake.back());
            PlaySound(state.appleSound);
        } else {
            state.snake.pop_back();
        }
    }
    
    // Spawn new apples
    if (state.gameMode == MODE_ACCELERATED) {
        for (int i = 0; i < 3 && state.apples.size() < GameConstants::MAX_APPLES; i++) {
            state.SpawnApple(state.gameTime);
        }
    } else {
        state.SpawnApple(state.gameTime);
    }
}

