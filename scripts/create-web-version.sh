#!/bin/bash

# Script to help create a web version of the Snake game
# This creates a template for a JavaScript/Canvas version

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

echo "Creating web version template..."

WEB_DIR="web"
mkdir -p "$WEB_DIR"

# Create a simple JavaScript/Canvas version template
cat > "$WEB_DIR/snake.html" <<'EOF'
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Snake Game - Web Version</title>
    <style>
        body {
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            background: #1a1a1a;
            font-family: 'Arial', sans-serif;
            color: white;
        }
        #game-container {
            text-align: center;
        }
        canvas {
            border: 3px solid #333;
            background: #000;
            display: block;
            margin: 0 auto;
        }
        #controls {
            margin-top: 20px;
            font-size: 14px;
            color: #aaa;
        }
        #score {
            font-size: 24px;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
    <div id="game-container">
        <div id="score">Score: 0 | High: 0</div>
        <canvas id="gameCanvas" width="720" height="800"></canvas>
        <div id="controls">
            <p>Arrow Keys / WASD - Move | P - Pause | R - Restart</p>
        </div>
    </div>

    <script>
        // TODO: Implement game logic here
        // This is a template - you'll need to port your C++ game logic
        
        const canvas = document.getElementById('gameCanvas');
        const ctx = canvas.getContext('2d');
        
        // Game state
        let score = 0;
        let highScore = 0;
        let gameOver = false;
        let paused = false;
        
        // Initialize game
        function init() {
            console.log('Snake Game - Web Version');
            console.log('Port your game logic from C++ to JavaScript here');
            
            // Example: Draw something
            ctx.fillStyle = '#fff';
            ctx.fillText('Snake Game - Web Version', 300, 400);
            ctx.fillText('Port your game logic here', 290, 430);
        }
        
        // Game loop
        function gameLoop() {
            if (!paused && !gameOver) {
                // Update game state
                update();
            }
            
            // Draw everything
            draw();
            
            requestAnimationFrame(gameLoop);
        }
        
        function update() {
            // Port your game update logic here
        }
        
        function draw() {
            // Clear canvas
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            
            // Draw game elements
            // Port your rendering code here
        }
        
        // Input handling
        document.addEventListener('keydown', (e) => {
            switch(e.key) {
                case 'ArrowUp':
                case 'w':
                case 'W':
                    // Handle up
                    break;
                case 'ArrowDown':
                case 's':
                case 'S':
                    // Handle down
                    break;
                case 'ArrowLeft':
                case 'a':
                case 'A':
                    // Handle left
                    break;
                case 'ArrowRight':
                case 'd':
                case 'D':
                    // Handle right
                    break;
                case 'p':
                case 'P':
                    paused = !paused;
                    break;
                case 'r':
                case 'R':
                    // Restart
                    break;
            }
        });
        
        // Start game
        init();
        gameLoop();
    </script>
</body>
</html>
EOF

echo "Created web template at: $WEB_DIR/snake.html"
echo ""
echo "Next steps:"
echo "1. Port your game logic from C++ to JavaScript"
echo "2. Implement rendering using Canvas API"
echo "3. Add sound using Web Audio API"
echo "4. Test locally: open web/snake.html in a browser"
echo "5. Deploy to GitHub Pages, Netlify, or Vercel"

