const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');


let x = 10, y = 10;
let dx = 0.2, dy = 0.2;
const size = 6; 
let color = '#ffffff';

// Helper to get random color
function getRandomColor() {
    const letters = '89ABCDEF';
    let col = '#';
    for (let i = 0; i < 6; i++) {
        col += letters[Math.floor(Math.random() * letters.length)];
    }
    return col;
}

function animate() {

    ctx.fillStyle = 'black';
    ctx.fillRect(0, 0, 64, 32);

    ctx.fillStyle = color;
    ctx.fillRect(Math.floor(x), Math.floor(y), size, size);

    x += dx;
    y += dy;

    let hit = false;
    if (x + size >= 64 || x <= 0) {
        dx = -dx;
        hit = true;
    }
    if (y + size >= 32 || y <= 0) {
        dy = -dy;
        hit = true;
    }

    if (hit) {
        color = getRandomColor();
    }

    requestAnimationFrame(animate);
}

animate();