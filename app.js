const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");
let display;

function animate() {
  ctx.fillStyle = "black";
  ctx.fillRect(0, 0, 64, 32);
  ctx.fillStyle = "white";

  if (typeof display !== "undefined") {
    for (let i = 0; i < 2048; i++) {
      if (display[i] !== 0) {
        const x = i % 64;
        const y = Math.floor(i / 64);
        ctx.fillRect(x, y, 1, 1);
      }
    }
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
  display = new Uint32Array(Module.HEAPU32.buffer, ptrDisplay, 2048);

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

function loadRom(){
  
}