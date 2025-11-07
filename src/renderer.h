#pragma once

#include "game_state.h"

class Renderer {
public:
    static void DrawModeSelectionScreen(const GameState& state);
    static void DrawInstructionsScreen();
    static void DrawGame(const GameState& state);
    static void DrawGameOverScreen(const GameState& state);
    static void DrawPauseScreen(const GameState& state);
    static void DrawResumeCountdown(const GameState& state);
};

