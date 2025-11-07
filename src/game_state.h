#pragma once

#include "game_types.h"
#include "raylib.h"
#include <vector>
#include <deque>

class GameState {
public:
    // Game mode and screens
    GameMode gameMode = MODE_REGULAR;
    bool showModeSelection = true;
    bool showInstructions = false;
    bool gameOver = false;
    int selectedModeIndex = 0;
    
    // Score
    int score = 0;
    int highScoreRegular = 0;
    int highScoreAccelerated = 0;
    
    // Helper to get current mode's high score
    int GetCurrentHighScore() const {
        return (gameMode == MODE_ACCELERATED) ? highScoreAccelerated : highScoreRegular;
    }
    
    // Helper to update current mode's high score
    void UpdateHighScore() {
        if (gameMode == MODE_ACCELERATED) {
            if (score > highScoreAccelerated) {
                highScoreAccelerated = score;
            }
        } else {
            if (score > highScoreRegular) {
                highScoreRegular = score;
            }
        }
    }
    
    // Snake
    std::vector<Position> snake;
    int dx = 0;
    int dy = 0;
    std::deque<Direction> directionQueue;
    float moveTimer = 0.0f;
    
    // Apples
    std::vector<Apple> apples;
    float gameTime = 0.0f;
    
    // Status effects
    bool canIntersectSelf = false;
    float immunityTimer = 0.0f;
    bool canPassWalls = false;
    float wallImmunityTimer = 0.0f;
    bool cannotEatApples = false;
    float cannotEatTimer = 0.0f;
    
    // Pause states
    bool isPaused = false;
    float pauseTimer = 0.0f;
    bool isUserPaused = false;
    bool isResuming = false;
    float resumeDelayTimer = 0.0f;
    
    // Sound timers
    float poisonSoundTimer = 0.0f;
    float pauseSoundTimer = 0.0f;
    bool gameOverSoundPlayed = false;
    
    // Sounds
    Sound appleSound;
    Sound poisonSound;
    Sound goldenSound;
    Sound purpleSound;
    Sound gameOverSound;
    Sound pauseSound;
    
    // Initialization
    void Initialize();
    void Cleanup();
    void Reset();
    
    // Apple management
    bool IsValidPosition(int col, int row) const;
    FoodType GetRandomFoodType() const;
    bool SpawnApple(float currentTime);
    
    // Status effect updates
    void UpdateStatusEffects(float deltaTime);
    void UpdateAppleDespawn(float deltaTime);
};

