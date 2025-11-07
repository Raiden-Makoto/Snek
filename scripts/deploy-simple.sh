#!/bin/bash

# Simple Cross-Platform Deployment Script
# Creates a simple folder with executable and assets

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

VERSION=${1:-"1.0"}
PLATFORM=${2:-$(uname -s)}

echo "Building game..."
./run.sh

echo "Creating distribution for ${PLATFORM}..."

case "$PLATFORM" in
    Darwin|*macOS*)
        PLATFORM_NAME="macOS"
        EXECUTABLE="snake"
        ;;
    Linux|*linux*)
        PLATFORM_NAME="Linux"
        EXECUTABLE="snake"
        ;;
    MINGW*|MSYS*|*Windows*)
        PLATFORM_NAME="Windows"
        EXECUTABLE="snake.exe"
        ;;
    *)
        PLATFORM_NAME="Unknown"
        EXECUTABLE="snake"
        ;;
esac

APP_NAME="SnakeGame"
DIST_DIR="dist/${APP_NAME}-${PLATFORM_NAME}"

rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"

echo "Copying executable..."
cp "build/${EXECUTABLE}" "$DIST_DIR/"

echo "Copying sounds..."
cp -r sounds "$DIST_DIR/"

echo "Creating README..."
cat > "$DIST_DIR/README.txt" <<EOF
Snake Game v${VERSION}

INSTRUCTIONS:
1. Make sure you have raylib installed on your system
2. Run: ./${EXECUTABLE} (or ${EXECUTABLE}.exe on Windows)

REQUIREMENTS:
- raylib library
- Sound system (for audio)

SOUND FILES:
The sounds/ directory must be in the same folder as the executable.

CONTROLS:
- Arrow Keys / WASD - Move
- P - Pause/Unpause
- Q - Quit or show game over
- ESC - Exit
- R / Space - Restart (on game over)
- M - Return to menu (on game over)
EOF

echo "Creating run script..."
if [[ "$PLATFORM_NAME" != "Windows" ]]; then
    cat > "$DIST_DIR/run.sh" <<'EOF'
#!/bin/bash
cd "$(dirname "$0")"
./snake
EOF
    chmod +x "$DIST_DIR/run.sh"
else
    cat > "$DIST_DIR/run.bat" <<EOF
@echo off
cd /d "%~dp0"
snake.exe
EOF
fi

echo "Creating archive..."
cd dist
if [[ "$PLATFORM_NAME" == "Windows" ]]; then
    zip -r "${APP_NAME}-${PLATFORM_NAME}-v${VERSION}.zip" "${APP_NAME}-${PLATFORM_NAME}/" || echo "zip not available, skipping archive"
else
    tar -czf "${APP_NAME}-${PLATFORM_NAME}-v${VERSION}.tar.gz" "${APP_NAME}-${PLATFORM_NAME}/" || echo "tar not available, skipping archive"
fi

echo "Deployment complete!"
echo "Distribution created at: dist/${APP_NAME}-${PLATFORM_NAME}/"
echo "Archive created at: dist/${APP_NAME}-${PLATFORM_NAME}-v${VERSION}.tar.gz"

