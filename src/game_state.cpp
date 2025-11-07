#include "game_state.h"
#include "raylib.h"
#include <ctime>
#include <algorithm>
#include <string>
#include <cstdio>

void GameState::Initialize() {
    // Try to find sounds directory (check multiple possible paths)
    // This handles different deployment scenarios
    std::string soundsPath = "sounds/";
    
    #ifdef PLATFORM_WEB
    // On web, preloaded files are at /sounds/
    soundsPath = "/sounds/";
    #else
    // Helper function to check if a path exists (simple check)
    auto fileExists = [](const char* path) -> bool {
        FILE* file = fopen(path, "r");
        if (file) {
            fclose(file);
            return true;
        }
        return false;
    };
    
    // Try different paths
    const char* testPaths[] = {
        "sounds/apple.mp3",           // Same directory as executable
        "../sounds/apple.mp3",        // Parent directory
        "../Resources/sounds/apple.mp3", // macOS app bundle
        "./sounds/apple.mp3",         // Current directory
    };
    
    for (const char* testPath : testPaths) {
        if (fileExists(testPath)) {
            // Extract directory path
            std::string path = testPath;
            size_t lastSlash = path.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                soundsPath = path.substr(0, lastSlash + 1);
            }
            break;
        }
    }
    #endif
    
    // Load sound effects (with error checking for web)
    appleSound = LoadSound((soundsPath + "apple.mp3").c_str());
    poisonSound = LoadSound((soundsPath + "poison.mp3").c_str());
    goldenSound = LoadSound((soundsPath + "golden.mp3").c_str());
    purpleSound = LoadSound((soundsPath + "purple.mp3").c_str());
    gameOverSound = LoadSound((soundsPath + "gameover.mp3").c_str());
    pauseSound = LoadSound((soundsPath + "pause.mp3").c_str());
    
    // Initialize random seed
    SetRandomSeed((unsigned int)std::time(nullptr));
    
    // Don't call Reset() here - we want to show mode selection screen at startup
    // Initialize only what's needed for first startup
    score = 0;
    gameOver = false;
    showModeSelection = true;  // Show mode selection at startup
    showInstructions = false;
    gameTime = 0.0f;
    selectedModeIndex = 0;
    
    // Initialize snake (will be reset when mode is selected)
    snake.clear();
    snake.push_back({GetRandomValue(0, GameConstants::GRID_WIDTH - 1), 
                     GetRandomValue(0, GameConstants::GRID_HEIGHT - 1)});
    
    // Don't initialize apples yet - will be done when mode is selected
    apples.clear();
    
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
    poisonSoundTimer = 0.0f;
    pauseSoundTimer = 0.0f;
    gameOverSoundPlayed = false;
}

void GameState::Cleanup() {
    UnloadSound(appleSound);
    UnloadSound(poisonSound);
    UnloadSound(goldenSound);
    UnloadSound(purpleSound);
    UnloadSound(gameOverSound);
    UnloadSound(pauseSound);
}

void GameState::Reset() {
    score = 0;
    gameOver = false;
    showModeSelection = false;
    showInstructions = false;
    gameTime = 0.0f;
    
    // Reset snake
    snake.clear();
    snake.push_back({GetRandomValue(0, GameConstants::GRID_WIDTH - 1), 
                     GetRandomValue(0, GameConstants::GRID_HEIGHT - 1)});
    
    // Reset apples
    apples.clear();
    if (gameMode == MODE_ACCELERATED) {
        for (int i = 0; i < 3; i++) {
            SpawnApple(0.0f);
        }
    } else {
        SpawnApple(0.0f);
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
    poisonSoundTimer = 0.0f;
    pauseSoundTimer = 0.0f;
    gameOverSoundPlayed = false;
}

bool GameState::IsValidPosition(int col, int row) const {
    // Check snake
    for (const auto& segment : snake) {
        if (col == segment.col && row == segment.row) {
            return false;
        }
    }
    // Check existing apples
    for (const auto& apple : apples) {
        if (col == apple.col && row == apple.row) {
            return false;
        }
    }
    return true;
}

FoodType GameState::GetRandomFoodType() const {
    int foodRoll = GetRandomValue(1, 100);
    if (foodRoll <= 4) {
        return POMME_PLUS;
    } else if (foodRoll <= 5) {
        return POMME_SUPREME;
    } else if (foodRoll <= 15) {
        return POISONOUS;
    } else if (foodRoll <= 18) {
        return TELEPORT;
    } else {
        return REGULAR;
    }
}

bool GameState::SpawnApple(float currentTime) {
    if (apples.size() >= GameConstants::MAX_APPLES) {
        return false;
    }
    
    // Try to find a valid position (max 100 attempts)
    int attempts = 0;
    int col, row;
    do {
        col = GetRandomValue(0, GameConstants::GRID_WIDTH - 1);
        row = GetRandomValue(0, GameConstants::GRID_HEIGHT - 1);
        attempts++;
    } while (!IsValidPosition(col, row) && attempts < 100);
    
    if (attempts >= 100) {
        return false;
    }
    
    Apple newApple;
    newApple.col = col;
    newApple.row = row;
    newApple.type = GetRandomFoodType();
    newApple.spawnTime = currentTime;
    newApple.despawnTime = GetRandomValue(GameConstants::DESPAWN_TIME_MIN, 
                                          GameConstants::DESPAWN_TIME_MAX);
    apples.push_back(newApple);
    return true;
}

void GameState::UpdateStatusEffects(float deltaTime) {
    if (canIntersectSelf) {
        immunityTimer -= deltaTime;
        if (immunityTimer <= 0.0f) {
            canIntersectSelf = false;
            immunityTimer = 0.0f;
        }
    }
    
    if (cannotEatApples) {
        cannotEatTimer -= deltaTime;
        
        poisonSoundTimer -= deltaTime;
        if (poisonSoundTimer <= 0.0f) {
            PlaySound(poisonSound);
            poisonSoundTimer = 1.0f;
        }
        
        if (cannotEatTimer <= 0.0f) {
            cannotEatApples = false;
            cannotEatTimer = 0.0f;
            poisonSoundTimer = 0.0f;
        }
    } else {
        poisonSoundTimer = 0.0f;
    }
    
    if (canPassWalls) {
        wallImmunityTimer -= deltaTime;
        if (wallImmunityTimer <= 0.0f) {
            canPassWalls = false;
            wallImmunityTimer = 0.0f;
        }
    }
    
    if (isPaused) {
        pauseTimer -= deltaTime;
        if (pauseTimer <= 0.0f) {
            isPaused = false;
            pauseTimer = 0.0f;
        }
    }
    
    if (isResuming) {
        resumeDelayTimer -= deltaTime;
        
        pauseSoundTimer -= deltaTime;
        if (pauseSoundTimer <= 0.0f) {
            PlaySound(pauseSound);
            pauseSoundTimer = 1.0f;
        }
        
        if (resumeDelayTimer <= 0.0f) {
            isResuming = false;
            resumeDelayTimer = 0.0f;
            pauseSoundTimer = 0.0f;
        }
    } else {
        pauseSoundTimer = 0.0f;
    }
}

void GameState::UpdateAppleDespawn(float deltaTime) {
    if (gameMode != MODE_ACCELERATED) {
        return;
    }
    
    // Remove apples that have exceeded their despawn time
    auto it = apples.begin();
    while (it != apples.end()) {
        float elapsed = gameTime - it->spawnTime;
        if (elapsed >= it->despawnTime) {
            it = apples.erase(it);
        } else {
            ++it;
        }
    }
    
    // Ensure at least MIN_APPLES apples are on the board
    while (apples.size() < GameConstants::MIN_APPLES) {
        if (!SpawnApple(gameTime)) {
            break;
        }
    }
}

