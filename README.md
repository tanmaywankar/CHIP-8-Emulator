# A Web Based Chip 8 Emulator
* Written in C++ and compiled to Wasm using `Emscripten`. 
* Uses vanilla javascipt for web and  `Tone.js` for tone generation.

> **for mobile:** currently it does not work well on mobile, cuz keyboard controls do not exist on phone yk.

## Technical Features
*   **Display:** Uses Html canvas to render the chip8's 64 x 32 screen.
*   **Audio:** Generates a simple tone as sound, as per the instruction by the soundTimer of chip8.
*   **Keyboard Keypad Mapping:** Direct binding of standard modern keyboards to the traditional 16-key hexadecimal layout (0-F).

## Key Mapping
The emulator maps a standard QWERTY keyboard to the original Chip8 4x4 hex keypad layout:
```text
Original Pad:          Modern Keyboard:
[1] [2] [3] [C]        [1] [2] [3] [4]
[4] [5] [6] [D]   -->  [Q] [W] [E] [R]
[7] [8] [9] [E]        [A] [S] [D] [F]
[A] [0] [B] [F]        [Z] [X] [C] [V]
```

## How to Build and Compile
**idk why someone would want to build this thing (unless you are a nerd like me), but if you do, then follow these steps:**

> **Note:** you do not need to follow the steps below, if you do not want to mess with the C++/Wasm code. you can just run the website using live server.

### Prerequesite:
* if you want to modify the c++ code then you would need C++ installed on your system along with the emscripten SDK.
* Follow this tutorial for emscripten installation: https://emscripten.org/docs/getting_started/downloads.html
### Compiling
* run the following command in the root directory of your project:
* ```emcc main.cpp CHIP8.cpp -o emu_core.js -O3 -s EXPORTED_RUNTIME_METHODS="['HEAPU8','HEAPU16','HEAPU32']"```
### Running locally
* you can run it like any normal web code, by using the liveserver plugin or similar.

## Project Folder Structure
```
chip8/
├── assets/
│   ├── keyboard.png
│   └── pc.png
├── roms/
│   ├── ibmlogo.ch8
│   ├── Pong.ch8
│   ├── Tank.ch8
│   └── test_opcode.ch8
├── .gitignore
├── app.js
├── CHIP8.cpp
├── CHIP8.hpp
├── emu_core.js
├── emu_core.wasm
├── index.html
├── main.cpp
└── style.css
```
## For adding roms:
* you need to put the given `.ch8` file in the roms folder. and then add its location in these two files:
  1. **As an option for dropdown:** in `<option value="name">rom_name</option>` in the `index.html` file.
  2. **As a path:** in the dropdown change function as: ```else if (value === "rom_name") {
    romPath = "roms/your_rom.ch8";
  }```
  
