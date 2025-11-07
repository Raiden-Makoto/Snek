# Snake Game

A classic Snake game built with C++ and raylib, featuring multiple apple types, status effects (inspired by Minecraft), and modern game mechanics (includes special sound effects!!).

## Features

### Core Gameplay
- **720x800 pixel window** with a 720x720 game board and an 80-pixel score display area
- **Checkerboard pattern** with alternating black and very dark gray (RGB 18, 18, 18) 30x30 unit cells
- **White border** (1 full unit thick) around the playable area
- **Smooth movement** at one unit every 0.25 seconds
- **Direction queue** to handle rapid key presses and prevent missed inputs
- **Snake growth** by 2 units for every apple eaten (except purple apples)

### Apple Types

1. **Regular Apple (Red)** - 82% spawn rate
   - Increases score by 1
   - Snake grows by 2 units

2. **Poisonous Apple (Light Brown)** - 10% spawn rate
   - Reverses snake direction
   - Pauses movement for 0.5 seconds
   - Applies "Poisoned" debuff for 10 seconds
   - During debuff: Cannot eat regular or purple apples (they disappear without effect)
   - Pomme Plus apples still work during debuff

3. **Pomme Plus (Orange)** - 4% spawn rate
   - Increases score by 2
   - Snake grows by 2 units
   - Grants "Resistance" for 10 seconds
   - Allows self-intersection (snake can pass through its own body)
   - Always works, even when poisoned

4. **Pomme Supreme (Yellow)** - 1% spawn rate
   - Increases score by 2
   - Snake grows by 2 units
   - Grants "Resistance II" for 10 seconds
   - Allows self-intersection AND wall-wrapping (snake wraps around edges)
   - Always works, even when poisoned

5. **Purple Apple (Purple)** - 3% spawn rate
   - Teleports snake to a random valid location
   - Randomly reorients the snake
   - Does NOT increase snake length
   - Pauses movement until user presses a key
   - Cannot be eaten when poisoned

### Controls

- **Arrow Keys** or **WASD** - Change snake direction
- **P** - Pause/unpause game (toggle)
- **Q** - Show game over screen (during gameplay) or quit (on game over screen)
- **ESC** - Exit game immediately
- **R** or **Space** - Restart game (on game over screen)

### Game Features

- **Score System**
  - Current score displayed in center of score area
  - High score displayed in top-right corner
  - Both scores shown on game over screen

- **Status Effect Display**
  - Real-time countdown timers in top-right corner
  - "Poisoned: X" - Light brown, shows remaining debuff time
  - "Resistance: X" - Orange, shows remaining self-intersection time
  - "Resistance II: X" - Yellow, shows remaining wall-wrapping time

- **Pause System**
  - Press P to pause/unpause
  - 2-second countdown before resuming
  - Status effect timers are frozen during pause and resume delay

- **Game Over Screen**
  - Displays final score and high score
  - Options to restart (R/Space) or quit (Q/ESC)
  - Can be triggered by collision or by pressing Q during gameplay

### Visual Design

- **Snake**: Green body with darker green head
- **Border**: White, 1 full unit thick on all sides
- **Background**: Checkerboard pattern (black and very dark gray)
- **Score Area**: Black background with white text
- **Game Over Overlay**: Semi-transparent black with centered text

## Building and Running

### Prerequisites

- CMake (version 3.10 or higher)
- raylib library
- C++ compiler (C++11 or higher)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./snake
```

Or use the provided script:

```bash
./run.sh
```

## Game Rules

1. The snake starts as a single segment at a random location
2. The snake does not move until the first key press
3. Eating regular/pomme plus apples increases score and length
4. Eating poisonous apples reverses direction and applies debuff
5. Eating purple apples teleports the snake (no growth)
6. Colliding with walls or self (without immunity) ends the game
7. Status effects are displayed with countdown timers
8. High score persists throughout the session

## Status Effects

- **Poisoned**: Cannot eat regular or purple apples for 10 seconds
- **Resistance**: Can intersect with own body for 10 seconds
- **Resistance II**: Can intersect with own body AND pass through walls for 10 seconds

## Notes

- All timers are frozen during pause and resume delay
- Pomme Plus apples always work, even when poisoned
- Purple apples maintain snake length when teleporting
- Direction queue prevents missed inputs from rapid key presses
- Snake head is drawn on top of body segments for clear collision visualization
