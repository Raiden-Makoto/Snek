#pragma once

#include "game_state.h"

class GameLogic {
public:
    static void ProcessMovement(GameState& state, float deltaTime);
    static void HandleAppleConsumption(GameState& state, int eatenAppleIndex);
    static void CheckCollisions(GameState& state, Position newHead);
    static void ProcessDirectionQueue(GameState& state);
};

