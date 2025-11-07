# Quick Deployment Guide

## For macOS (Current Platform)

### Option 1: Simple Distribution (Recommended for Testing)

```bash
# Build the game
./run.sh

# Create distribution folder
mkdir -p dist/SnakeGame-macOS
cp build/snake dist/SnakeGame-macOS/
cp -r sounds dist/SnakeGame-macOS/

# Create zip file
cd dist
zip -r SnakeGame-macOS.zip SnakeGame-macOS/
```

**Distribution Contents:**
- `snake` - The game executable
- `sounds/` - All sound files

**Requirements:**
- Users need raylib installed: `brew install raylib`
- Or bundle raylib dylib with the distribution

### Option 2: Use Deployment Script

```bash
# Simple deployment (creates folder with executable and sounds)
./scripts/deploy-simple.sh

# macOS App Bundle (creates .app bundle)
./scripts/deploy-macos.sh
```

### Option 3: Static Linking (Easier Distribution)

Build with static linking to avoid raylib dependency:

```bash
./scripts/build-static.sh
```

Then copy the executable and sounds folder.

## For GitHub Releases

1. **Create a tag:**
   ```bash
   git tag v1.0
   git push origin v1.0
   ```

2. **Use GitHub Actions:**
   - The `.github/workflows/build.yml` will automatically build and create releases
   - Or manually upload the distribution folder

3. **Manual Upload:**
   - Go to GitHub Releases
   - Create a new release
   - Upload `SnakeGame-macOS.zip`

## Testing Distribution

Before distributing, test on a clean system:

```bash
# Copy to a test location
cp -r dist/SnakeGame-macOS ~/Desktop/test-snake

# Test from there
cd ~/Desktop/test-snake
./snake
```

## Cross-Platform Deployment

### Windows
- Requires Windows build environment (MinGW or MSVC)
- Bundle raylib DLL with the executable
- Create installer with Inno Setup or NSIS

### Linux
- Build on Linux or use Docker
- Bundle raylib library or use system package
- Create `.deb` or `.AppImage` package

## Notes

- Sound files are automatically found in multiple locations (same directory, parent directory, macOS app bundle Resources)
- The game will work as long as `sounds/` folder is in the same directory as the executable
- For production, consider code signing on macOS
- Consider creating an installer for easier distribution

