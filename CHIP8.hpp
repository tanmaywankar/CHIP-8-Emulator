#include <cstdint>
#include <random>
class Chip8{

    public:
    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint8_t keypad[16]{};
    static const unsigned int DISPLAY_WIDTH = 64;
    static const unsigned int DISPLAY_HEIGHT = 32;
    uint32_t display[DISPLAY_HEIGHT * DISPLAY_WIDTH]{};
    uint16_t opcode{};
    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    Chip8();
    
void LoadROM(char const* filename);
void Cycle();

};