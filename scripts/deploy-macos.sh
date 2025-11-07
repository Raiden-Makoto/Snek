#!/bin/bash

# macOS Deployment Script
# Creates a distributable macOS package

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

VERSION=${1:-"1.0"}
APP_NAME="SnakeGame"
BUNDLE_NAME="${APP_NAME}.app"
DIST_DIR="dist/macOS"
CONTENTS_DIR="${DIST_DIR}/${BUNDLE_NAME}/Contents"
MACOS_DIR="${CONTENTS_DIR}/MacOS"
RESOURCES_DIR="${CONTENTS_DIR}/Resources"

echo "Building game..."
./run.sh

echo "Creating app bundle structure..."
rm -rf "$DIST_DIR"
mkdir -p "$MACOS_DIR"
mkdir -p "$RESOURCES_DIR"

echo "Copying executable..."
cp build/snake "$MACOS_DIR/${APP_NAME}"

echo "Copying sounds..."
cp -r sounds "$RESOURCES_DIR/"

echo "Creating Info.plist..."
cat > "${CONTENTS_DIR}/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>${APP_NAME}</string>
    <key>CFBundleIdentifier</key>
    <string>com.snakegame.app</string>
    <key>CFBundleName</key>
    <string>${APP_NAME}</string>
    <key>CFBundleVersion</key>
    <string>${VERSION}</string>
    <key>CFBundleShortVersionString</key>
    <string>${VERSION}</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.9</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF

echo "Updating executable to use Resources/sounds..."
# We need to update the code to look for sounds in Resources, or create a wrapper
# For now, create a symlink
cd "$MACOS_DIR"
ln -sf "../Resources/sounds" sounds

echo "Bundling raylib (if needed)..."
# Check if raylib is statically or dynamically linked
if otool -L "${APP_NAME}" | grep -q libraylib; then
    echo "Dynamic linking detected - you may need to bundle raylib dylib"
    echo "Or use static linking for easier distribution"
fi

echo "Creating DMG (optional)..."
cd "$PROJECT_ROOT/$DIST_DIR"
hdiutil create -volname "${APP_NAME}" -srcfolder "${BUNDLE_NAME}" -ov -format UDZO "${APP_NAME}-macOS-v${VERSION}.dmg" 2>/dev/null || echo "DMG creation skipped (hdiutil not available or error)"

echo "Deployment complete!"
echo "App bundle created at: ${DIST_DIR}/${BUNDLE_NAME}"
echo "To distribute: zip -r ${APP_NAME}-macOS-v${VERSION}.zip ${BUNDLE_NAME}"

