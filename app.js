const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

//placeholder code until the emu core works

let x = 10,
  y = 10;
let dx = 0.2,
  dy = 0.2;
const size = 6;
let color = "#ffffff";

// Helper to get random color
function getRandomColor() {
  const letters = "89ABCDEF";
  let col = "#";
  for (let i = 0; i < 6; i++) {
    col += letters[Math.floor(Math.random() * letters.length)];
  }
  return col;
}

function animate() {
  ctx.fillStyle = "black";
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

//actual code starts

Module.onRuntimeInitialized = () => {
  const ptrRegister = Module._getRegisterPointer();
  const ptrPC = Module._getPCPointer();
  const ptrIndex = Module._getIndexPointer();
  const ptrOpcode = Module._getOpcodePointer();
  const ptrDisplay = Module._getDisplayPointer();

  const registers = new Uint8Array(Module.HEAPU8.buffer, ptrRegister, 16);
  const pc = new Uint16Array(Module.HEAPU16.buffer, ptrPC, 1);
  const index = new Uint16Array(Module.HEAPU16.buffer, ptrIndex, 1);
  const opcode = new Uint16Array(Module.HEAPU16.buffer, ptrOpcode, 1);

  function toHex(num, padding) {
    return "0x" + num.toString(16).toUpperCase().padStart(padding, 0);
  }
  setTimeout(() => {
    document.getElementById("ui-pc").innerText = toHex(pc[0], 4);
    document.getElementById("ui-index").innerText = toHex(index[0], 4);
    document.getElementById("ui-instruction").innerText = toHex(opcode[0], 4);
  }, 10);

  console.log(index[0]);

  setTimeout(() => {
    const regContainer = document.getElementById("ui-registers");
    if (regContainer) {
      regContainer.innerHTML = "";
      for (let i = 0; i < 16; i++) {
        const hexLabel = i.toString(16).toUpperCase();
        const hexValue = toHex(registers[i], 2);
        console.log(registers[15]);
        regContainer.innerHTML += `<div class = "register-values"><b>V${hexLabel}</b>: ${hexValue}</div>`;
      }
    }
  }, 10);
};
