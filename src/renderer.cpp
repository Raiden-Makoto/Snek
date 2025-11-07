#include "renderer.h"
#include "game_types.h"
#include "raylib.h"
#include <string>
#include <cmath>

void Renderer::DrawModeSelectionScreen(const GameState& state) {
    ClearBackground(BLACK);
    
    // Title
    const int titleFontSize = 60;
    std::string titleText = "SNAKE GAME";
    int titleWidth = MeasureText(titleText.c_str(), titleFontSize);
    int titleX = (GameConstants::SCREEN_WIDTH - titleWidth) / 2;
    int titleY = 150;
    DrawText(titleText.c_str(), titleX, titleY, titleFontSize, WHITE);
    
    // Subtitle
    const int subtitleFontSize = 32;
    std::string subtitleText = "Select Game Mode";
    int subtitleWidth = MeasureText(subtitleText.c_str(), subtitleFontSize);
    int subtitleX = (GameConstants::SCREEN_WIDTH - subtitleWidth) / 2;
    int subtitleY = titleY + 80;
    DrawText(subtitleText.c_str(), subtitleX, subtitleY, subtitleFontSize, YELLOW);
    
    // Mode options
    const int modeFontSize = 36;
    const int modeSpacing = 80;
    int modeStartY = subtitleY + 100;
    int modeX = GameConstants::SCREEN_WIDTH / 2;
    
    // Regular mode
    Color regularColor = (state.selectedModeIndex == 0) ? GREEN : LIGHTGRAY;
    std::string regularText = "Regular";
    int regularTextWidth = MeasureText(regularText.c_str(), modeFontSize);
    int regularX = modeX - regularTextWidth / 2;
    int regularY = modeStartY;
    DrawText(regularText.c_str(), regularX, regularY, modeFontSize, regularColor);
    
    // Accelerated mode
    Color acceleratedColor = (state.selectedModeIndex == 1) ? GREEN : LIGHTGRAY;
    std::string acceleratedText = "Accelerated";
    int acceleratedTextWidth = MeasureText(acceleratedText.c_str(), modeFontSize);
    int acceleratedX = modeX - acceleratedTextWidth / 2;
    int acceleratedY = modeStartY + modeSpacing;
    DrawText(acceleratedText.c_str(), acceleratedX, acceleratedY, modeFontSize, acceleratedColor);
    
    // Selection indicator
    const int arrowSize = 20;
    int arrowX = modeX - 150;
    int arrowY = modeStartY + (state.selectedModeIndex * modeSpacing) + (modeFontSize - arrowSize) / 2;
    DrawText(">", arrowX, arrowY, arrowSize, GREEN);
    
    // Instructions
    const int instructionFontSize = 20;
    std::string instructionText = "Use UP/DOWN or W/S to select, SPACE or ENTER to confirm";
    int instructionWidth = MeasureText(instructionText.c_str(), instructionFontSize);
    int instructionX = (GameConstants::SCREEN_WIDTH - instructionWidth) / 2;
    int instructionY = acceleratedY + modeSpacing + 40;
    DrawText(instructionText.c_str(), instructionX, instructionY, instructionFontSize, LIGHTGRAY);
}

void Renderer::DrawInstructionsScreen() {
    ClearBackground(BLACK);
    
    // Title
    const int titleFontSize = 50;
    std::string titleText = "SNAKE GAME";
    int titleWidth = MeasureText(titleText.c_str(), titleFontSize);
    int titleX = (GameConstants::SCREEN_WIDTH - titleWidth) / 2;
    int titleY = 40;
    DrawText(titleText.c_str(), titleX, titleY, titleFontSize, WHITE);
    
    // Instructions header
    const int headerFontSize = 32;
    std::string headerText = "APPLE TYPES";
    int headerWidth = MeasureText(headerText.c_str(), headerFontSize);
    int headerX = (GameConstants::SCREEN_WIDTH - headerWidth) / 2;
    int headerY = titleY + 70;
    DrawText(headerText.c_str(), headerX, headerY, headerFontSize, YELLOW);
    
    // Apple type instructions
    const int textFontSize = 20;
    int startY = headerY + 50;
    int lineHeight = 28;
    int leftMargin = 40;
    int currentY = startY;
    
    // Regular Apple
    DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, RED);
    std::string regularText = "Regular Apple (Red) - 82%: Score +1, Grow +2 units";
    DrawText(regularText.c_str(), leftMargin, currentY, textFontSize, WHITE);
    currentY += lineHeight;
    
    // Poisonous Apple
    DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, GameConstants::POISON_COLOR);
    std::string poisonText = "Poisonous Apple (Brown) - 10%: Reverses direction, 10s debuff";
    DrawText(poisonText.c_str(), leftMargin, currentY, textFontSize, WHITE);
    std::string poisonSubText = "  Cannot eat regular/purple apples during debuff";
    DrawText(poisonSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
    currentY += lineHeight * 2;
    
    // Pomme Plus
    DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, GameConstants::GOLD_COLOR);
    std::string pommePlusText = "Pomme Plus (Orange) - 4%: Score +2, Resistance 10s";
    DrawText(pommePlusText.c_str(), leftMargin, currentY, textFontSize, WHITE);
    std::string pommePlusSubText = "  Can pass through own body, works when poisoned";
    DrawText(pommePlusSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
    currentY += lineHeight * 2;
    
    // Pomme Supreme
    DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, GameConstants::ENCHANTED_GOLD_COLOR);
    std::string pommeText = "Pomme Supreme (Yellow) - 1%: Score +2, Resistance II 10s";
    DrawText(pommeText.c_str(), leftMargin, currentY, textFontSize, WHITE);
    std::string pommeSubText = "  Pass through body + walls, works when poisoned";
    DrawText(pommeSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
    currentY += lineHeight * 2;
    
    // Purple Apple
    DrawRectangle(leftMargin - 35, currentY - 2, 25, 25, GameConstants::PURPLE_COLOR);
    std::string purpleText = "Purple Apple (Purple) - 3%: Teleport to random location";
    DrawText(purpleText.c_str(), leftMargin, currentY, textFontSize, WHITE);
    std::string purpleSubText = "  No growth, cannot be eaten when poisoned";
    DrawText(purpleSubText.c_str(), leftMargin + 10, currentY + lineHeight - 5, textFontSize - 2, LIGHTGRAY);
    currentY += lineHeight * 2 + 20;
    
    // Controls header
    std::string controlsHeader = "CONTROLS";
    int controlsHeaderWidth = MeasureText(controlsHeader.c_str(), headerFontSize);
    int controlsHeaderX = (GameConstants::SCREEN_WIDTH - controlsHeaderWidth) / 2;
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
    std::string startText = "Press SPACE or ENTER to start";
    int startTextWidth = MeasureText(startText.c_str(), textFontSize + 4);
    int startTextX = (GameConstants::SCREEN_WIDTH - startTextWidth) / 2;
    DrawText(startText.c_str(), startTextX, currentY, textFontSize + 4, GREEN);
}

void Renderer::DrawGame(const GameState& state) {
    // Clear screen
    ClearBackground(BLACK);
    
    // Draw score area background
    DrawRectangle(0, 0, GameConstants::SCREEN_WIDTH, GameConstants::SCORE_AREA_HEIGHT, BLACK);
    
    // Draw score text
    const int fontSize = 40;
    std::string scoreText = "Score: " + std::to_string(state.score);
    int textWidth = MeasureText(scoreText.c_str(), fontSize);
    int textX = (GameConstants::SCREEN_WIDTH - textWidth) / 2;
    int textY = (GameConstants::SCORE_AREA_HEIGHT - fontSize) / 2;
    DrawText(scoreText.c_str(), textX, textY, fontSize, WHITE);
    
    // Draw high score text
    const int highScoreFontSize = 24;
    std::string highScoreText = "High: " + std::to_string(state.GetCurrentHighScore());
    int highScoreX = GameConstants::SCREEN_WIDTH - MeasureText(highScoreText.c_str(), highScoreFontSize) - 20;
    int highScoreY = (GameConstants::SCORE_AREA_HEIGHT - highScoreFontSize) / 2;
    DrawText(highScoreText.c_str(), highScoreX, highScoreY, highScoreFontSize, LIGHTGRAY);
    
    // Draw status effects
    const int statusFontSize = 18;
    int statusY = highScoreY + highScoreFontSize + 5;
    int statusRightMargin = 20;
    
    if (state.cannotEatApples && state.cannotEatTimer > 0.0f) {
        int countdown = (int)std::ceil(state.cannotEatTimer);
        std::string statusText = "Poisoned: " + std::to_string(countdown);
        int statusX = GameConstants::SCREEN_WIDTH - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
        DrawText(statusText.c_str(), statusX, statusY, statusFontSize, GameConstants::POISON_COLOR);
        statusY += statusFontSize + 3;
    }
    
    if (state.canIntersectSelf && state.immunityTimer > 0.0f) {
        int countdown = (int)std::ceil(state.immunityTimer);
        std::string statusText = "Resistance: " + std::to_string(countdown);
        int statusX = GameConstants::SCREEN_WIDTH - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
        DrawText(statusText.c_str(), statusX, statusY, statusFontSize, GameConstants::GOLD_COLOR);
        statusY += statusFontSize + 3;
    }
    
    if (state.canPassWalls && state.wallImmunityTimer > 0.0f) {
        int countdown = (int)std::ceil(state.wallImmunityTimer);
        std::string statusText = "Resistance II: " + std::to_string(countdown);
        int statusX = GameConstants::SCREEN_WIDTH - MeasureText(statusText.c_str(), statusFontSize) - statusRightMargin;
        DrawText(statusText.c_str(), statusX, statusY, statusFontSize, GameConstants::ENCHANTED_GOLD_COLOR);
    }
    
    // Draw white border
    int boardStartY = GameConstants::BOARD_START_Y;
    int cellSize = GameConstants::CELL_SIZE;
    for (int col = 0; col < GameConstants::TOTAL_GRID_WIDTH; col++) {
        DrawRectangle(col * cellSize, boardStartY, cellSize, cellSize, WHITE);
    }
    for (int col = 0; col < GameConstants::TOTAL_GRID_WIDTH; col++) {
        DrawRectangle(col * cellSize, boardStartY + (GameConstants::TOTAL_GRID_HEIGHT - 1) * cellSize, cellSize, cellSize, WHITE);
    }
    for (int row = 1; row < GameConstants::TOTAL_GRID_HEIGHT - 1; row++) {
        DrawRectangle(0, boardStartY + row * cellSize, cellSize, cellSize, WHITE);
    }
    for (int row = 1; row < GameConstants::TOTAL_GRID_HEIGHT - 1; row++) {
        DrawRectangle((GameConstants::TOTAL_GRID_WIDTH - 1) * cellSize, boardStartY + row * cellSize, cellSize, cellSize, WHITE);
    }
    
    // Draw checkerboard
    for (int row = 0; row < GameConstants::GRID_HEIGHT; row++) {
        for (int col = 0; col < GameConstants::GRID_WIDTH; col++) {
            int screenCol = (col + GameConstants::BORDER_OFFSET) * cellSize;
            int screenRow = boardStartY + (row + GameConstants::BORDER_OFFSET) * cellSize;
            if ((row + col) % 2 == 0) {
                DrawRectangle(screenCol, screenRow, cellSize, cellSize, BLACK);
            } else {
                DrawRectangle(screenCol, screenRow, cellSize, cellSize, GameConstants::GRAY_COLOR);
            }
        }
    }
    
    // Draw apples
    for (const auto& apple : state.apples) {
        Color foodColor;
        if (apple.type == POMME_SUPREME) {
            foodColor = GameConstants::ENCHANTED_GOLD_COLOR;
        } else if (apple.type == POMME_PLUS) {
            foodColor = GameConstants::GOLD_COLOR;
        } else if (apple.type == POISONOUS) {
            foodColor = GameConstants::POISON_COLOR;
        } else if (apple.type == TELEPORT) {
            foodColor = GameConstants::PURPLE_COLOR;
        } else {
            foodColor = RED;
        }
        DrawRectangle((apple.col + GameConstants::BORDER_OFFSET) * cellSize, 
                     boardStartY + (apple.row + GameConstants::BORDER_OFFSET) * cellSize, 
                     cellSize, cellSize, foodColor);
    }
    
    // Draw snake
    for (size_t i = 1; i < state.snake.size(); i++) {
        const auto& segment = state.snake[i];
        DrawRectangle((segment.col + GameConstants::BORDER_OFFSET) * cellSize, 
                     boardStartY + (segment.row + GameConstants::BORDER_OFFSET) * cellSize, 
                     cellSize, cellSize, GameConstants::SNAKE_COLOR);
    }
    
    if (!state.snake.empty()) {
        const auto& head = state.snake[0];
        DrawRectangle((head.col + GameConstants::BORDER_OFFSET) * cellSize, 
                     boardStartY + (head.row + GameConstants::BORDER_OFFSET) * cellSize, 
                     cellSize, cellSize, GameConstants::SNAKE_HEAD_COLOR);
    }
}

void Renderer::DrawGameOverScreen(const GameState& state) {
    DrawRectangle(0, 0, GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, {0, 0, 0, 180});
    
    const int gameOverFontSize = 60;
    std::string gameOverText = "GAME OVER";
    int gameOverTextWidth = MeasureText(gameOverText.c_str(), gameOverFontSize);
    int gameOverX = (GameConstants::SCREEN_WIDTH - gameOverTextWidth) / 2;
    int gameOverY = GameConstants::SCREEN_HEIGHT / 2 - 100;
    DrawText(gameOverText.c_str(), gameOverX, gameOverY, gameOverFontSize, WHITE);
    
    const int finalScoreFontSize = 40;
    std::string finalScoreText = "Final Score: " + std::to_string(state.score);
    int finalScoreTextWidth = MeasureText(finalScoreText.c_str(), finalScoreFontSize);
    int finalScoreX = (GameConstants::SCREEN_WIDTH - finalScoreTextWidth) / 2;
    int finalScoreY = gameOverY + 80;
    DrawText(finalScoreText.c_str(), finalScoreX, finalScoreY, finalScoreFontSize, WHITE);
    
    std::string highScoreText = "High Score: " + std::to_string(state.GetCurrentHighScore());
    int highScoreTextWidth = MeasureText(highScoreText.c_str(), finalScoreFontSize);
    int highScoreX = (GameConstants::SCREEN_WIDTH - highScoreTextWidth) / 2;
    int highScoreY = finalScoreY + 60;
    DrawText(highScoreText.c_str(), highScoreX, highScoreY, finalScoreFontSize, YELLOW);
    
    const int instructionFontSize = 24;
    std::string restartText = "Press R or SPACE to restart";
    std::string menuText = "Press M to return to menu";
    std::string quitText = "Press ESC to exit or Q to quit";
    int restartTextWidth = MeasureText(restartText.c_str(), instructionFontSize);
    int menuTextWidth = MeasureText(menuText.c_str(), instructionFontSize);
    int quitTextWidth = MeasureText(quitText.c_str(), instructionFontSize);
    int restartX = (GameConstants::SCREEN_WIDTH - restartTextWidth) / 2;
    int menuX = (GameConstants::SCREEN_WIDTH - menuTextWidth) / 2;
    int quitX = (GameConstants::SCREEN_WIDTH - quitTextWidth) / 2;
    int instructionY = highScoreY + 80;
    DrawText(restartText.c_str(), restartX, instructionY, instructionFontSize, LIGHTGRAY);
    DrawText(menuText.c_str(), menuX, instructionY + 35, instructionFontSize, LIGHTGRAY);
    DrawText(quitText.c_str(), quitX, instructionY + 70, instructionFontSize, LIGHTGRAY);
}

void Renderer::DrawPauseScreen(const GameState& state) {
    DrawRectangle(0, 0, GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, {0, 0, 0, 180});
    
    const int pauseFontSize = 60;
    std::string pauseText = "PAUSED";
    int pauseTextWidth = MeasureText(pauseText.c_str(), pauseFontSize);
    int pauseX = (GameConstants::SCREEN_WIDTH - pauseTextWidth) / 2;
    int pauseY = GameConstants::SCREEN_HEIGHT / 2 - 30;
    DrawText(pauseText.c_str(), pauseX, pauseY, pauseFontSize, WHITE);
    
    const int instructionFontSize = 24;
    std::string resumeText = "Press P to resume (or Q to quit)";
    int resumeTextWidth = MeasureText(resumeText.c_str(), instructionFontSize);
    int resumeX = (GameConstants::SCREEN_WIDTH - resumeTextWidth) / 2;
    DrawText(resumeText.c_str(), resumeX, pauseY + 80, instructionFontSize, LIGHTGRAY);
}

void Renderer::DrawResumeCountdown(const GameState& state) {
    DrawRectangle(0, 0, GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT, {0, 0, 0, 180});
    
    const int resumeFontSize = 40;
    int countdown = (int)std::ceil(state.resumeDelayTimer);
    std::string resumeText = "Resuming in " + std::to_string(countdown) + "...";
    int resumeTextWidth = MeasureText(resumeText.c_str(), resumeFontSize);
    int resumeX = (GameConstants::SCREEN_WIDTH - resumeTextWidth) / 2;
    int resumeY = GameConstants::SCREEN_HEIGHT / 2;
    DrawText(resumeText.c_str(), resumeX, resumeY, resumeFontSize, WHITE);
}

