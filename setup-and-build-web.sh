#!/bin/bash
# Master script to set up and build everything for web

set -e

cd "$(dirname "$0")"

echo "=========================================="
echo "Snake Game - Web Build Setup"
echo "=========================================="
echo ""

# Step 1: Check for Emscripten
echo "Step 1: Checking for Emscripten..."
if command -v emcc &> /dev/null; then
    echo "✓ Emscripten found: $(which emcc)"
    emcc --version | head -1
else
    echo "✗ Emscripten not found!"
    echo ""
    echo "Installing Emscripten..."
    echo "(This may take several minutes)"
    echo ""
    
    # Check if emsdk already exists
    if [ -d "emsdk" ]; then
        echo "✓ emsdk directory already exists"
        cd emsdk
    elif [ -d "$HOME/emsdk" ]; then
        echo "✓ emsdk found at $HOME/emsdk"
        cd "$HOME/emsdk"
    else
        echo "Cloning emsdk repository..."
        git clone https://github.com/emscripten-core/emsdk.git
        cd emsdk
    fi
    
    echo "Installing latest Emscripten..."
    ./emsdk install latest
    
    echo "Activating Emscripten..."
    ./emsdk activate latest
    
    echo "Activating for current session..."
    source ./emsdk_env.sh
    
    cd "$(dirname "$0")"
    
    # Verify installation
    if ! command -v emcc &> /dev/null; then
        echo ""
        echo "ERROR: Emscripten installation failed!"
        echo "Please run manually:"
        echo "  source emsdk/emsdk_env.sh"
        echo "Then run this script again."
        exit 1
    fi
    
    echo "✓ Emscripten installed and activated!"
    emcc --version | head -1
fi

echo ""

# Step 2: Check/build raylib
echo "Step 2: Checking for raylib (Emscripten build)..."
RAYLIB_BUILT=false

# Check for both libraylib.a and libraylib.web.a (raylib web build uses .web.a)
if [ -f "raylib/src/libraylib.web.a" ]; then
    echo "✓ Found raylib at raylib/src/libraylib.web.a"
    RAYLIB_BUILT=true
elif [ -f "raylib/src/libraylib.a" ]; then
    echo "✓ Found raylib at raylib/src/libraylib.a"
    RAYLIB_BUILT=true
elif [ -f "build-web/libraylib.web.a" ]; then
    echo "✓ Found raylib at build-web/libraylib.web.a"
    RAYLIB_BUILT=true
elif [ -f "build-web/libraylib.a" ]; then
    echo "✓ Found raylib at build-web/libraylib.a"
    RAYLIB_BUILT=true
elif [ -f "$HOME/raylib/src/libraylib.web.a" ]; then
    echo "✓ Found raylib at $HOME/raylib/src/libraylib.web.a"
    RAYLIB_BUILT=true
elif [ -f "$HOME/raylib/src/libraylib.a" ]; then
    echo "✓ Found raylib at $HOME/raylib/src/libraylib.a"
    RAYLIB_BUILT=true
fi

if [ "$RAYLIB_BUILT" = false ]; then
    echo "✗ raylib not found for Emscripten"
    echo ""
    echo "Building raylib for web..."
    echo ""
    
    if [ ! -d "raylib" ]; then
        echo "Cloning raylib repository..."
        git clone https://github.com/raysan5/raylib.git
    fi
    
    cd raylib/src
    echo "Building raylib with Emscripten (this may take a few minutes)..."
    emmake make PLATFORM=PLATFORM_WEB -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    # Raylib web build creates libraylib.web.a
    if [ -f "libraylib.web.a" ]; then
        echo "✓ raylib built successfully! (libraylib.web.a)"
        cd ../..
        RAYLIB_BUILT=true
    elif [ -f "libraylib.a" ]; then
        echo "✓ raylib built successfully! (libraylib.a)"
        cd ../..
        RAYLIB_BUILT=true
    else
        echo "✗ Failed to build raylib!"
        echo "Expected libraylib.web.a or libraylib.a in raylib/src/"
        cd ../..
        exit 1
    fi
fi

echo ""

# Step 3: Build the game
echo "Step 3: Building Snake Game for WebAssembly..."
echo ""

# Create build directory
mkdir -p build-web
cd build-web

# Copy sounds directory
cp -r ../sounds .

# Find raylib
RAYLIB_LIB=""
RAYLIB_INC=""

# Check common locations for raylib (check for both .a and .web.a)
if [ -f "../raylib/src/libraylib.web.a" ]; then
    RAYLIB_LIB="../raylib/src/libraylib.web.a"
    RAYLIB_INC="-I../raylib/src"
elif [ -f "../raylib/src/libraylib.a" ]; then
    RAYLIB_LIB="../raylib/src/libraylib.a"
    RAYLIB_INC="-I../raylib/src"
elif [ -f "libraylib.web.a" ]; then
    RAYLIB_LIB="libraylib.web.a"
elif [ -f "libraylib.a" ]; then
    RAYLIB_LIB="libraylib.a"
elif [ -f "$HOME/raylib/src/libraylib.web.a" ]; then
    RAYLIB_LIB="$HOME/raylib/src/libraylib.web.a"
    RAYLIB_INC="-I$HOME/raylib/src"
elif [ -f "$HOME/raylib/src/libraylib.a" ]; then
    RAYLIB_LIB="$HOME/raylib/src/libraylib.a"
    RAYLIB_INC="-I$HOME/raylib/src"
elif [ -d "/usr/local/lib" ] && [ -f "/usr/local/lib/libraylib.a" ]; then
    RAYLIB_LIB="/usr/local/lib/libraylib.a"
    RAYLIB_INC="-I/usr/local/include"
fi

if [ -z "$RAYLIB_LIB" ]; then
    echo ""
    echo "ERROR: raylib not found for Emscripten!"
    echo ""
    echo "This should have been built in Step 2. Something went wrong."
    exit 1
fi

echo "Found raylib at: $RAYLIB_LIB"
if [ -n "$RAYLIB_INC" ]; then
    echo "Using raylib includes: $RAYLIB_INC"
fi

# Compile sources
echo "Compiling C++ sources with Emscripten..."
emcc \
    ../src/main.cpp \
    ../src/game_state.cpp \
    ../src/game_logic.cpp \
    ../src/renderer.cpp \
    -I../src \
    $RAYLIB_INC \
    -std=c++17 \
    -s USE_GLFW=3 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s INITIAL_MEMORY=67108864 \
    -s MAX_WEBGL_VERSION=2 \
    -s MIN_WEBGL_VERSION=2 \
    -s USE_WEBGL2=1 \
    -s FULL_ES3=1 \
    -s ASSERTIONS=1 \
    -s USE_SDL=0 \
    --preload-file sounds@/sounds \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -s EXPORTED_FUNCTIONS='["_main"]' \
    -s USE_PTHREADS=0 \
    -s PROXY_TO_PTHREAD=0 \
    -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -O2 \
    -DPLATFORM_WEB \
    -o snake.html \
    "$RAYLIB_LIB" \
    2>&1

if [ $? -ne 0 ]; then
    echo ""
    echo "Build failed! Check the errors above."
    exit 1
fi

cd ..

echo ""
echo "=========================================="
echo "Build Complete!"
echo "=========================================="
echo ""
echo "To test the game:"
echo "  1. cd build-web"
echo "  2. python3 -m http.server 8000"
echo "  3. Open http://localhost:8000/snake.html"
echo ""

