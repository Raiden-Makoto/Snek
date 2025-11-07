#!/bin/bash

# Build Snake game for web using Emscripten

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
    echo "Error: Emscripten not found!"
    echo "Please install Emscripten:"
    echo "  git clone https://github.com/emscripten-core/emsdk.git"
    echo "  cd emsdk"
    echo "  ./emsdk install latest"
    echo "  ./emsdk activate latest"
    echo "  source ./emsdk_env.sh"
    exit 1
fi

echo "Building for web..."

# Create web build directory
mkdir -p build-web
cd build-web

# Note: Raylib web support may require special setup
# This is a template - you may need to adjust based on raylib's web port

echo "Warning: This script is a template."
echo "Raylib's web support may require:"
echo "  1. Building raylib with Emscripten"
echo "  2. Using raylib's web port"
echo "  3. Or using an alternative web framework"
echo ""
echo "Consider porting to JavaScript/TypeScript for better web compatibility."

# Uncomment when raylib web is set up:
# emcmake cmake .. -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake
# emmake make

echo ""
echo "Alternative: Use a web game framework like:"
echo "  - Phaser.js"
echo "  - PixiJS"
echo "  - Three.js"
echo "  - Babylon.js"
echo ""
echo "Or use raylib's official web port if available."

