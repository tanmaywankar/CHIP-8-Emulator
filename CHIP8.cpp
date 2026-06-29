#include "CHIP8.hpp"
#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>
#include <fstream>

const unsigned int start_address = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::LoadROM(char const* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if(file.is_open()){
        std::streampos size = file.tellg();
        std::vector<char> buffer(size);

        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);
        file.close();

        for (long i = 0; i < size; ++i){
            memory[start_address + i] = buffer[i];
        }
    }
    
    else{
        std::cout<<"file could not be opened"<<std::endl;
    }
}

Chip8::Chip8(): randGen(std::chrono::system_clock::now().time_since_epoch().count()){
    pc = start_address;

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

}

void Chip8::Cycle(){
    opcode = (memory[pc] << 8) | memory[pc + 1]; 
    pc += 2;

    uint8_t first_digit = (opcode & 0xF000) >> 12;
    uint8_t x = (opcode & 0xF000) >> 8;
    uint8_t y = (opcode & 0xF000) >> 4;
    uint8_t kk = (opcode & 0x00FF);
    uint8_t last_digit = (opcode & 0x00FF);

    switch (first_digit)
    {
    case 0x0: 
        switch(kk){
            case 0xE0:
            std::memset(display, 0, sizeof(display));
            break;
            case 0xEE:
            --sp;
            pc = stack[sp];
            break;
        }
        break;
    
    case 0x1:
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
    break;

    case 0x2:
    ++sp;
    pc = stack[sp];
    break;

    

    
    default:
    std::cout<<"error"<<std::endl;
        break;
    }

}