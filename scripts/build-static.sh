#!/bin/bash

# Build with static linking for easier distribution
# This reduces dependencies but increases binary size

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "Building with static linking..."

mkdir -p build-static
cd build-static

# Try to build with static raylib
# Note: This requires raylib to be built statically
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF

make -j$(sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Build complete!"
echo "Executable: build-static/snake"
echo "Note: You may need to adjust CMakeLists.txt for static linking depending on your raylib installation"

