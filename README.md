# Snake Game

A Snake game implementation in C++ using raylib.

## Prerequisites

- CMake (version 3.11 or higher)
- A C++ compiler with C++17 support
- raylib library

### Installing raylib

#### macOS (using Homebrew)
```bash
brew install raylib
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libraylib-dev
```

#### Building from source
If raylib is not available via package manager, you can build it from source:
```bash
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./snake
```

The game window should open showing a 720x720 black screen.

