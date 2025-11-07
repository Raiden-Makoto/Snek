# Deployment Guide

This guide covers how to deploy the Snake game for different platforms.

## Overview

The game requires:
- The `snake` executable
- The `sounds/` directory with all MP3 files
- raylib library (bundled or system-installed)

## Platform-Specific Deployment

### macOS

#### Option 1: Simple Distribution (Command Line)
1. Build the game:
   ```bash
   ./run.sh
   ```

2. Create a distribution folder:
   ```bash
   mkdir -p dist/SnakeGame-macOS
   cp build/snake dist/SnakeGame-macOS/
   cp -r sounds dist/SnakeGame-macOS/
   ```

3. Bundle raylib (if using dynamic linking):
   ```bash
   # Copy raylib dylib
   cp /opt/homebrew/opt/raylib/lib/libraylib.*.dylib dist/SnakeGame-macOS/
   ```

4. Create a run script:
   ```bash
   echo '#!/bin/bash
   cd "$(dirname "$0")"
   ./snake' > dist/SnakeGame-macOS/run.sh
   chmod +x dist/SnakeGame-macOS/run.sh
   ```

5. Zip for distribution:
   ```bash
   cd dist
   zip -r SnakeGame-macOS.zip SnakeGame-macOS/
   ```

#### Option 2: macOS App Bundle
Create a proper `.app` bundle for macOS using the `deploy-macos.sh` script.

### Windows

#### Requirements
- CMake
- MinGW-w64 or Visual Studio
- raylib Windows library

#### Steps
1. Build with CMake (use MinGW or MSVC)
2. Copy `snake.exe` and `sounds/` folder to distribution folder
3. Bundle raylib DLLs
4. Create installer (optional) with Inno Setup or NSIS

### Linux

#### Requirements
- CMake
- raylib (system-installed or bundled)
- Sound system (ALSA/PulseAudio)

#### Steps
1. Build the game
2. Bundle sounds directory
3. Optionally create a `.deb` or `.AppImage` package

## Quick Deploy Scripts

Use the provided scripts in the `scripts/` directory:
- `deploy-macos.sh` - Creates macOS distribution
- `deploy-windows.sh` - Creates Windows distribution (requires cross-compilation)
- `deploy-linux.sh` - Creates Linux distribution

## GitHub Releases

1. Build for each platform
2. Create zip files for each platform
3. Upload to GitHub Releases with names like:
   - `SnakeGame-macOS-v1.0.zip`
   - `SnakeGame-Windows-v1.0.zip`
   - `SnakeGame-Linux-v1.0.zip`

## Notes

- Sound files must be in `sounds/` directory relative to the executable
- raylib must be available (bundled or installed)
- On macOS, you may need to code sign the app for distribution outside the App Store

