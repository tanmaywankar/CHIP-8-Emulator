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

void Chip8::LoadROM(char const *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        std::vector<char> buffer(size);

        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);
        file.close();

        for (long i = 0; i < size; ++i)
        {
            memory[start_address + i] = buffer[i];
        }
    }

    else
    {
        std::cout << "file could not be opened" << std::endl;
    }
}

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    pc = start_address;

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::Cycle()
{
    opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;

    uint8_t first_digit = (opcode & 0xF000u) >> 12u;
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    uint8_t kk = (opcode & 0x00FFu);
    uint8_t last_digit = (opcode & 0x000Fu);
    uint16_t address = opcode & 0x0FFFu;

    switch (first_digit)
    {
    case 0x0:
        switch (kk)
        {
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
        pc = address;
        break;

    case 0x2:
        stack[sp] = pc;
        ++sp;
        pc = address;
        break;

    case 0x3:
        if (registers[x] == kk)
        {
            pc += 2;
        }
        break;

    case 0x4:
        if (registers[x] != kk)
        {
            pc += 2;
        }
        break;

    case 0x5:
    {
        if (registers[x] == registers[y])
        {
            pc += 2;
        }
    }

    break;

    case 0x6:
    {
        registers[x] = kk;
    }
    break;

    case 0x7:
        registers[x] += kk;
        break;

    case 0x8:
        switch (last_digit)
        {
        case 0x0:
            registers[x] = registers[y];
            break;

        case 0x1:
            registers[x] |= registers[y];
            break;

        case 0x2:
            registers[x] &= registers[y];
            break;

        case 0x3:
            registers[x] ^= registers[y];
            break;

        case 0x4:
        {
            uint16_t sum = registers[x] + registers[y];

            if (sum > 255U)
            {
                registers[0xF] = 1;
            }
            else
            {
                registers[0xF] = 0;
            }
            registers[x] = sum & 0xFFU;
        }
        break;

        case 0x5:
            if (registers[x] > registers[y])
            {
                registers[0xF] = 1;
            }
            else
            {
                registers[0xF] = 0;
            }
            registers[x] -= registers[y];
            break;

        case 0x6:
            registers[0xF] = (registers[x] & 0x1u);
            registers[x] >>= 1;
            break;

        case 0x7:
            if (registers[y] > registers[x])
            {
                registers[0xF] = 1;
            }
            else
            {
                registers[0xF] = 0;
            }
            registers[x] = registers[y] - registers[x];
            break;

        case 0xE:
            registers[0xF] = (registers[x] & 0x80u) >> 7u;
            registers[x] <<= 1;
            break;
        }
        break;

    case 0x9:
        if (registers[x] != registers[y])
        {
            pc += 2;
        }
        break;

    case 0xA:
    {
        index = address;
    }
    break;

    case 0xB:
    {
        pc = registers[0] + address;
    }
    break;

    case 0xC:
        registers[x] = randByte(randGen) & kk;
        break;

    case 0xD:
    {
        uint8_t xpos = registers[x] % DISPLAY_WIDTH;
        uint8_t ypos = registers[y] % DISPLAY_HEIGHT;

        registers[0xF] = 0;

        for (unsigned int row = 0; row < last_digit; ++row)
        {
            uint8_t spriteByte = memory[index + row];

            for (unsigned int column = 0; column < 8; ++column)
            {
                // its 1000 0000 so everytime we do bit shifting to left, like column 1, it becomes 0100 0000 and so on, and then we put on or off per pixel, in series.
                uint8_t spritePixel = spriteByte & (0x80u >> column);

                uint32_t *screenPixel = &display[(ypos + row) * DISPLAY_WIDTH + (xpos + column)];

                if (spritePixel)
                {
                    if (*screenPixel == 0xFFFFFFFF)
                    {
                        registers[0xF] = 1;
                    }

                    *screenPixel ^= 0xFFFFFFFF;
                }
            }
        }
    }
    break;
    case 0xE:
    {
        switch (kk)
        {
        case 0x9E:
        {
            uint8_t key = registers[x];
            if (keypad[key])
            {
                pc += 2;
            }
        }
        break;

        case 0xA1:
        {
            uint8_t key = registers[x];
            if (!keypad[key])
            {
                pc += 2;
            }
            break;
        }
        }
    }
    break;

    case 0xF:
        switch (kk)
        {
        case 0x07:
            registers[x] = delayTimer;
            break;

        case 0x0A:
        {
            if (keypad[0])
            {
                registers[x] = 0;
            }
            else if (keypad[1])
            {
                registers[x] = 1;
            }
            else if (keypad[2])
            {
                registers[x] = 2;
            }
            else if (keypad[3])
            {
                registers[x] = 3;
            }
            else if (keypad[4])
            {
                registers[x] = 4;
            }
            else if (keypad[5])
            {
                registers[x] = 5;
            }
            else if (keypad[6])
            {
                registers[x] = 6;
            }
            else if (keypad[7])
            {
                registers[x] = 7;
            }
            else if (keypad[8])
            {
                registers[x] = 8;
            }
            else if (keypad[9])
            {
                registers[x] = 9;
            }
            else if (keypad[10])
            {
                registers[x] = 10;
            }
            else if (keypad[11])
            {
                registers[x] = 11;
            }
            else if (keypad[12])
            {
                registers[x] = 12;
            }
            else if (keypad[13])
            {
                registers[x] = 13;
            }
            else if (keypad[14])
            {
                registers[x] = 14;
            }
            else if (keypad[15])
            {
                registers[x] = 15;
            }
            else
            {
                pc -= 2;
            }
        }
        break;

        case 0x15:
            delayTimer = registers[x];
            break;

        case 0x18:
            soundTimer = registers[x];
            break;

        case 0x1E:
            index += registers[x];
            break;

        case 0x29:
        {
            uint8_t digit = registers[x];
            index = FONTSET_START_ADDRESS + (5 * digit);
        }
        break;

        case 0x33:
        {
            uint8_t value = registers[x];

            memory[index + 2] = value % 10;
            value /= 10;

            memory[index + 1] = value % 10;
            value /= 10;

            memory[index] = value % 10;
        }
        break;

        case 0x55:
            for (uint8_t i = 0; i <= x; ++i)
            {
                memory[index + i] = registers[i];
            }
            break;

        case 0x65:
        {
            for (uint8_t i = 0; i <= x; ++i)
            {
                registers[i] = memory[index + i];
            }
        }
        break;
        }

        break;

    default:
        std::cerr << "[CPU ERROR] Unknown Opcode: 0x"
                  << std::hex << std::uppercase << opcode
                  << " at Memory Address (PC): 0x" << pc
                  << std::dec << std::endl;

        exit(1);

        break;
    }
}