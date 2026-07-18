const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");
let display;
let pc;
let index;
let opcode;
let registers;
let isRomLoaded = false;
let romPath = "roms/ibmlogo.ch8";

let keypad;
const KEY_MAP = {
  1: 0x1,
  2: 0x2,
  3: 0x3,
  4: 0xc,
  q: 0x4,
  w: 0x5,
  e: 0x6,
  r: 0xd,
  a: 0x7,
  s: 0x8,
  d: 0x9,
  f: 0xe,
  z: 0xa,
  x: 0x0,
  c: 0xb,
  v: 0xf,
};

function toHex(num, padding) {
  return "0x" + num.toString(16).toUpperCase().padStart(padding, 0);
}

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
  if (pc && index && opcode) {
    document.getElementById("ui-pc").innerText = toHex(pc[0], 4);
    document.getElementById("ui-index").innerText = toHex(index[0], 4);
    document.getElementById("ui-instruction").innerText = toHex(opcode[0], 4);

    const regContainer = document.getElementById("ui-registers");
    if (regContainer) {
      regContainer.innerHTML = "";
      for (let i = 0; i < 16; i++) {
        const hexLabel = i.toString(16).toUpperCase();
        const hexValue = toHex(registers[i], 2);
        regContainer.innerHTML += `<div class = "register-values"><b>V${hexLabel}</b>: ${hexValue}</div>`;
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
  const ptrKeypad = Module._getKeyPointer();

  registers = new Uint8Array(Module.HEAPU8.buffer, ptrRegister, 16);
  pc = new Uint16Array(Module.HEAPU16.buffer, ptrPC, 1);
  index = new Uint16Array(Module.HEAPU16.buffer, ptrIndex, 1);
  opcode = new Uint16Array(Module.HEAPU16.buffer, ptrOpcode, 1);
  display = new Uint32Array(Module.HEAPU32.buffer, ptrDisplay, 2048);
  keypad = new Uint8Array(Module.HEAPU8.buffer, ptrKeypad, 16);

  window.addEventListener("keydown", (e) => {
    const inputKey = KEY_MAP[e.key.toLowerCase()];

    if (inputKey !== undefined) {
      keypad[inputKey] = 1; 
    }
  });

  window.addEventListener("keyup", (e) => {
    const inputKey = KEY_MAP[e.key.toLowerCase()];

    if (inputKey !== undefined) {
      keypad[inputKey] = 0;
    }
  });

  loadRom(romPath, Module, pc);
};

function loadRom(rom, WasmModule, pcArray) {
  fetch(rom)
    .then((response) => {
      if (!response.ok)
        throw new error(`did not find the file: ${response.statusText}`);
      return response.arrayBuffer();
    })
    .then((buffer) => {
      const romData = new Uint8Array(buffer);
      console.log(`rom found : ${romData.length} bytes`);
      const ptrMemory = WasmModule._getMemoryPointer();
      const emuMemory = new Uint8Array(
        WasmModule.HEAPU8.buffer,
        ptrMemory + 0x0200,
        romData.length,
      );
      emuMemory.set(romData);

      if (pcArray) {
        pcArray[0] = 0x0200;
      }

      console.log("program fed into memory");
      isRomLoaded = true;
    })

    .catch((error) => console.log("error loading", error));
}

document.getElementById("dropdown").onchange = (e) => {
  const value = e.target.value;
  if (value === "none") {
    romPath = "roms/ibmlogo.ch8";
  } else if (value === "pong") {
    romPath = "roms/Pong.ch8";
  } else if (value === "tank") {
    romPath = "roms/Tank.ch8";
  } else if (value === "test") {
    romPath = "roms/test_opcode.ch8";
  }

  if (typeof Module !== "undefined" && pc) {
    isRomLoaded = false;
    if (typeof display !== "undefined") {
      display.fill(0);
    }
    Module._resetEmulator();
    loadRom(romPath, Module, pc);
    e.target.blur();
  }
};
