# Snake Game - Web Version

This is the JavaScript/Canvas version of the Snake game, ready to deploy to any web server.

## Features

✅ All game features from the C++ version:
- Regular, Poisonous, Pomme Plus, Pomme Supreme, and Purple apples
- Status effects (Poisoned, Resistance, Resistance II)
- Mode selection (Regular/Accelerated)
- Separate high scores for each mode
- Pause/resume with countdown
- Game over screen with menu option
- Sound effects
- All game mechanics

## Files

- `snake.html` - Complete game (single file, includes all JavaScript)
- `sounds/` - Sound effects directory (required)

## Testing Locally

**Important:** The game must be served from a web server (not opened directly as `file://`) due to browser security restrictions for loading audio files.

### Option 1: Python HTTP Server (Recommended)
```bash
cd web
python3 -m http.server 8000
# Then visit http://localhost:8000/snake.html
```

### Option 2: Node.js HTTP Server
```bash
cd web
npx http-server -p 8000
# Then visit http://localhost:8000/snake.html
```

### Option 3: PHP Server
```bash
cd web
php -S localhost:8000
# Then visit http://localhost:8000/snake.html
```

**Note:** If you get 404 errors, make sure:
1. The `sounds/` directory is in the same folder as `snake.html`
2. You're accessing via `http://localhost:8000/snake.html` (not `file://`)
3. The web server is running from the `web/` directory

## Deployment

### GitHub Pages

1. Push the `web/` folder contents to your repository
2. Enable GitHub Pages in repository settings
3. Set source to the folder containing `snake.html`

### Netlify

1. Drag and drop the `web/` folder to Netlify
2. Done! Your game is live

### Vercel

1. Install Vercel CLI: `npm i -g vercel`
2. Run `vercel` in the `web/` directory
3. Follow the prompts

### Any Static Host

Upload the contents of the `web/` folder to any static hosting service.

## Browser Compatibility

- Chrome/Edge: ✅ Full support
- Firefox: ✅ Full support
- Safari: ✅ Full support
- Mobile browsers: ✅ Works (keyboard input may vary)

## Controls

- **Arrow Keys / WASD** - Move snake
- **P** - Pause/Unpause
- **Q** - Show game over screen (or quit)
- **ESC** - Exit (closes browser tab)
- **R / Space** - Restart (on game over)
- **M** - Return to menu (on game over)

## Notes

- Sound files must be in the `sounds/` directory relative to `snake.html`
- The game uses Web Audio API for sound (may require user interaction to start)
- All game logic is contained in a single HTML file for easy deployment

