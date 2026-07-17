#include <iostream>
#include <emscripten.h>
#include "CHIP8.hpp"

Chip8 chip8;

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    uint32_t *getDisplayPointer()
    {
        return chip8.display;
    }

    EMSCRIPTEN_KEEPALIVE
    uint8_t *getRegisterPointer()
    {
        return chip8.registers;
    }

    EMSCRIPTEN_KEEPALIVE
    uint16_t *getPCPointer()
    {
        return &chip8.pc;
    }

    EMSCRIPTEN_KEEPALIVE
    uint16_t *getOpcodePointer()
    {
        return &chip8.opcode;
    }
    EMSCRIPTEN_KEEPALIVE
    uint16_t *getIndexPointer()
    {
        return &chip8.index;
    }
    EMSCRIPTEN_KEEPALIVE
    uint8_t *getMemoryPointer(){
        return chip8.memory;
    }
}

void main_loop_tick(void *args)
{
}

int main()
{
    chip8.pc = 0x0200;
    chip8.index = 0x0A2B;
    chip8.opcode = 0xD015;

    chip8.registers[0] = 0x1A;
    chip8.registers[15] = 0xFF;

    emscripten_set_main_loop_arg(main_loop_tick, nullptr, 60, 1);
    return 0;
}