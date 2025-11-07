#pragma once

#include "raylib.h"
#include <vector>

struct Position {
    int col;
    int row;
};

struct Direction {
    int dx;
    int dy;
};

enum FoodType { REGULAR, POISONOUS, POMME_PLUS, POMME_SUPREME, TELEPORT };
enum GameMode { MODE_REGULAR, MODE_ACCELERATED };

struct Apple {
    int col;
    int row;
    FoodType type;
    float spawnTime;
    float despawnTime;
};

// Game constants
namespace GameConstants {
    const int BOARD_SIZE = 720;
    const int SCORE_AREA_HEIGHT = 80;
    const int SCREEN_WIDTH = 720;
    const int SCREEN_HEIGHT = BOARD_SIZE + SCORE_AREA_HEIGHT;
    const int CELL_SIZE = 30;
    const int GRID_WIDTH = (BOARD_SIZE / CELL_SIZE) - 2;
    const int GRID_HEIGHT = (BOARD_SIZE / CELL_SIZE) - 2;
    const int BORDER_OFFSET = 1;
    const int BOARD_START_Y = SCORE_AREA_HEIGHT;
    const int TOTAL_GRID_WIDTH = BOARD_SIZE / CELL_SIZE;
    const int TOTAL_GRID_HEIGHT = BOARD_SIZE / CELL_SIZE;
    
    // Colors
    const Color GRAY_COLOR = {18, 18, 18, 255};
    const Color SNAKE_COLOR = {50, 150, 50, 255};
    const Color SNAKE_HEAD_COLOR = {25, 100, 25, 255};
    const Color POISON_COLOR = {181, 126, 107, 255};
    const Color GOLD_COLOR = {255, 165, 0, 255};
    const Color ENCHANTED_GOLD_COLOR = {255, 255, 0, 255};
    const Color PURPLE_COLOR = {186, 85, 211, 255};
    
    // Game timing
    const float MOVE_INTERVAL_REGULAR = 0.25f;
    const float MOVE_INTERVAL_ACCELERATED = 0.20f;
    const float IMMUNITY_DURATION = 10.0f;
    const float WALL_IMMUNITY_DURATION = 10.0f;
    const float CANNOT_EAT_DURATION = 10.0f;
    const float PAUSE_DURATION = 0.5f;
    const float RESUME_DELAY_DURATION = 2.0f;
    
    // Apple settings
    const int MAX_APPLES = 12;
    const int MIN_APPLES = 2;
    const int DESPAWN_TIME_MIN = 13;
    const int DESPAWN_TIME_MAX = 18;
}

