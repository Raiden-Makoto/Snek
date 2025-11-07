#include "raylib.h"
#include "game_state.h"
#include "game_logic.h"
#include "renderer.h"
#include "game_types.h"
#include <deque>

int main() {
    // Initialize window first (required for web)
    InitWindow(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, "Snake Game");
    
    #ifdef PLATFORM_WEB
    // GL context initialization is handled by gl_init_post.js
    // Just poll events to let GLFW finish setup
    PollInputEvents();
    #endif
    
    // Initialize audio after window
    InitAudioDevice();
    SetTargetFPS(60);
    
    // Initialize game state
    GameState state;
    state.Initialize();
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Handle ESC (always exits)
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        // Handle mode selection screen
        if (state.showModeSelection) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                state.selectedModeIndex = 0;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                state.selectedModeIndex = 1;
            }
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                state.gameMode = (state.selectedModeIndex == 0) ? MODE_REGULAR : MODE_ACCELERATED;
                state.showModeSelection = false;
                state.showInstructions = true;
                
                // Initialize apples based on game mode
                state.apples.clear();
                if (state.gameMode == MODE_ACCELERATED) {
                    for (int i = 0; i < 3; i++) {
                        state.SpawnApple(0.0f);
                    }
                } else {
                    state.SpawnApple(0.0f);
                }
            }
            
            BeginDrawing();
            Renderer::DrawModeSelectionScreen(state);
            EndDrawing();
            continue;
        }
        
        // Handle instructions screen
        if (state.showInstructions) {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                state.showInstructions = false;
                state.gameTime = 0.0f;
            }
            
            BeginDrawing();
            Renderer::DrawInstructionsScreen();
            EndDrawing();
            continue;
        }
        
        float deltaTime = GetFrameTime();
        
        // Update game time
        if (!state.isUserPaused && !state.isResuming) {
            state.gameTime += deltaTime;
        }
        
        // Update status effects and apple despawn
        if (!state.isUserPaused && !state.isResuming) {
            state.UpdateStatusEffects(deltaTime);
        }
        state.UpdateAppleDespawn(deltaTime);
        
        // Handle input
        if (IsKeyPressed(KEY_Q)) {
            if (!state.gameOver) {
                state.gameOver = true;
            } else {
                break;
            }
        }
        
        if (!state.gameOver && IsKeyPressed(KEY_P)) {
            if (state.isUserPaused) {
                state.isResuming = true;
                state.resumeDelayTimer = GameConstants::RESUME_DELAY_DURATION;
                state.pauseSoundTimer = 1.0f;
                state.isUserPaused = false;
            } else if (!state.isResuming) {
                state.isUserPaused = true;
            }
        }
        
        // Handle game over restart and menu
        if (state.gameOver) {
            if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_SPACE)) {
                state.Reset();
            }
            if (IsKeyPressed(KEY_M)) {
                // Return to mode selection menu
                state.gameOver = false;
                state.showModeSelection = true;
                state.showInstructions = false;
                state.score = 0;
                // Reset game state but keep high scores
                state.snake.clear();
                state.apples.clear();
                state.dx = 0;
                state.dy = 0;
                state.directionQueue.clear();
                state.moveTimer = 0.0f;
                state.gameTime = 0.0f;
                state.canIntersectSelf = false;
                state.immunityTimer = 0.0f;
                state.canPassWalls = false;
                state.wallImmunityTimer = 0.0f;
                state.cannotEatApples = false;
                state.cannotEatTimer = 0.0f;
                state.isPaused = false;
                state.pauseTimer = 0.0f;
                state.isUserPaused = false;
                state.isResuming = false;
                state.resumeDelayTimer = 0.0f;
                state.poisonSoundTimer = 0.0f;
                state.pauseSoundTimer = 0.0f;
                state.gameOverSoundPlayed = false;
            }
        }
        
        // Handle movement input
        if (!state.gameOver && !state.isUserPaused && !state.isResuming) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                Direction newDir = {0, -1};
                if (((state.dx == 0 && state.dy == 0) || state.dy != 1) && 
                    (state.directionQueue.empty() || 
                     state.directionQueue.back().dx != newDir.dx || 
                     state.directionQueue.back().dy != newDir.dy)) {
                    state.directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                Direction newDir = {0, 1};
                if (((state.dx == 0 && state.dy == 0) || state.dy != -1) && 
                    (state.directionQueue.empty() || 
                     state.directionQueue.back().dx != newDir.dx || 
                     state.directionQueue.back().dy != newDir.dy)) {
                    state.directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                Direction newDir = {-1, 0};
                if (((state.dx == 0 && state.dy == 0) || state.dx != 1) && 
                    (state.directionQueue.empty() || 
                     state.directionQueue.back().dx != newDir.dx || 
                     state.directionQueue.back().dy != newDir.dy)) {
                    state.directionQueue.push_back(newDir);
                }
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                Direction newDir = {1, 0};
                if (((state.dx == 0 && state.dy == 0) || state.dx != -1) && 
                    (state.directionQueue.empty() || 
                     state.directionQueue.back().dx != newDir.dx || 
                     state.directionQueue.back().dy != newDir.dy)) {
                    state.directionQueue.push_back(newDir);
                }
            }
        }
        
        // Process game logic
        GameLogic::ProcessMovement(state, deltaTime);
        
        // Draw everything
        BeginDrawing();
        Renderer::DrawGame(state);
        
        if (state.isUserPaused && !state.gameOver) {
            Renderer::DrawPauseScreen(state);
        }
        
        if (state.isResuming && !state.gameOver) {
            Renderer::DrawResumeCountdown(state);
        }
        
        if (state.gameOver) {
            Renderer::DrawGameOverScreen(state);
        }
        
        EndDrawing();
    }
    
    // Cleanup
    state.Cleanup();
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}

