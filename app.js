const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");
let display;
let pc;
let index;
let opcode;
let registers;
let isRomLoaded = false;

function toHex(num, padding) {
  return "0x" + num.toString(16).toUpperCase().padStart(padding, 0);
}

function animate() {
  ctx.fillStyle = "black";
  ctx.fillRect(0, 0, 64, 32);
  ctx.fillStyle = "white";

  if (isRomLoaded && typeof Module._main_loop_tick !== "undefined") {
    for (let cycle = 0; cycle < 10; cycle++) {
      Module._main_loop_tick();
    }
  }

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
        console.log(registers[15]);
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

  registers = new Uint8Array(Module.HEAPU8.buffer, ptrRegister, 16);
  pc = new Uint16Array(Module.HEAPU16.buffer, ptrPC, 1);
  index = new Uint16Array(Module.HEAPU16.buffer, ptrIndex, 1);
  opcode = new Uint16Array(Module.HEAPU16.buffer, ptrOpcode, 1);
  display = new Uint32Array(Module.HEAPU32.buffer, ptrDisplay, 2048);
  loadRom("roms/test_opcode.ch8", Module, pc);

  console.log(index[0]);
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
