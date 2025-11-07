# Deploying to Vercel

## Important Notes

**raylib and emsdk are in .gitignore** - They are not committed to the repository. This is fine because:

1. They can be rebuilt from source during deployment
2. They are large dependencies (hundreds of MB)
3. Vercel will build them during the deployment process

## Deployment Steps

### Option 1: Using Vercel CLI (Recommended)

1. **Install Vercel CLI:**
   ```bash
   npm i -g vercel
   ```

2. **Deploy:**
   ```bash
   vercel
   ```

3. **Follow the prompts** - Vercel will use the `vercel.json` configuration

### Option 2: GitHub Integration

1. Push your code to GitHub
2. Connect your repo to Vercel
3. Vercel will automatically build and deploy using `vercel.json`

## How It Works

The `vercel.json` file configures:

1. **Build Command**: Runs `setup-and-build-web.sh` which:
   - Installs Emscripten automatically (if not present)
   - Builds raylib for web
   - Compiles the game to WebAssembly
   - Outputs to `build-web/`

2. **Output Directory**: `build-web/` (contains snake.html, snake.js, snake.wasm)

3. **Headers**: Sets correct MIME types for `.wasm` and `.data` files

**Note**: `raylib/` and `emsdk/` are in `.gitignore` - they will be downloaded and built during the Vercel build process. This is fine because:
- They're large dependencies (500MB+)
- They can be rebuilt from source
- Vercel's build environment has all the tools needed (git, make, python)

## Build Time

**Important**: The build will take 5-10 minutes because:
- Emscripten installation (~200MB download)
- Raylib compilation from source
- Game compilation to WebAssembly

This is normal and only happens during deployment.

## Troubleshooting

**Build timeout**
→ Vercel has a 45-minute build timeout. If it times out, consider:
- Using Vercel Pro (longer timeout)
- Pre-building raylib and committing it (not recommended, but possible)

**Emscripten not found**
→ The build command should install it automatically. If it fails, check Vercel build logs.

**Raylib build fails**
→ Make sure the build command has access to `git` and `make` commands (Vercel provides these).

**WASM files not loading**
→ Check that the MIME type headers are correct in `vercel.json`.

## Alternative: Pre-build and Commit

If build times are an issue, you could:

1. Remove `raylib/` and `emsdk/` from `.gitignore` (NOT RECOMMENDED - large files)
2. Build locally
3. Commit the built files
4. Deploy

**Warning**: This will make your repo very large (500MB+). Better to let Vercel build it.

## Environment Variables

No environment variables needed for basic deployment.

