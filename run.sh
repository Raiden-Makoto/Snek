#!/bin/bash

# Build and run the Snake game

set -e  # Exit on error

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure and build
echo "Configuring CMake..."
cmake .. > /dev/null

echo "Building..."
make

# Copy sounds directory to build if it doesn't exist
if [ ! -d "sounds" ]; then
    echo "Copying sounds directory to build..."
    cp -r ../sounds .
fi

# Run the program
echo "Running snake game..."
./snake

