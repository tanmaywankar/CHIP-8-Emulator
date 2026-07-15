#include <iostream>
#include <SDL2/SDL.h>
#include <emscripten.h>
#include "CHIP8.hpp"

const int scale = 15;
const int SCREEN_WIDTH = 64 * scale;
const int SCREEN_HEIGHT = 32 * scale;

void RenderDisplay(Chip8 &chip8, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
    SDL_RenderClear(renderer);

    for (int y = 0; y < 32; ++y)
    {
        for (int x = 0; x < 64; ++x)
        {
            SDL_Rect pixelRect;
            pixelRect.x = x * scale;
            pixelRect.y = y * scale;
            pixelRect.w = scale - 1;
            pixelRect.h = scale - 1;

            if (chip8.display[(y * 64) + x] == 0xFFFFFFFF)
            {
                SDL_SetRenderDrawColor(renderer, 0x2f, 0xc9, 0x00, 0xff);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
            }

            SDL_RenderFillRect(renderer, &pixelRect);
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Error! not enough argument, put some roms bro";
        return 1;
    }
    char const *romFile = argv[1];
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Chip8 chip8;
    chip8.LoadROM(romFile);

    bool quit = false;
    SDL_Event e;
    uint32_t lastCycleTime = SDL_GetTicks();

    uint32_t lastFpsTime = SDL_GetTicks();
    int frameCount = 0;


    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                bool isPressed = (e.type == SDL_KEYDOWN);

                switch (e.key.keysym.sym)
                {
                case SDLK_1:
                    chip8.keypad[0x1] = isPressed;
                    break;
                case SDLK_2:
                    chip8.keypad[0x2] = isPressed;
                    break;
                case SDLK_3:
                    chip8.keypad[0x3] = isPressed;
                    break;
                case SDLK_4:
                    chip8.keypad[0xC] = isPressed;
                    break;

                case SDLK_q:
                    chip8.keypad[0x4] = isPressed;
                    break;
                case SDLK_w:
                    chip8.keypad[0x5] = isPressed;
                    break;
                case SDLK_e:
                    chip8.keypad[0x6] = isPressed;
                    break;
                case SDLK_r:
                    chip8.keypad[0xD] = isPressed;
                    break;

                case SDLK_a:
                    chip8.keypad[0x7] = isPressed;
                    break;
                case SDLK_s:
                    chip8.keypad[0x8] = isPressed;
                    break;
                case SDLK_d:
                    chip8.keypad[0x9] = isPressed;
                    break;
                case SDLK_f:
                    chip8.keypad[0xE] = isPressed;
                    break;

                case SDLK_z:
                    chip8.keypad[0xA] = isPressed;
                    break;
                case SDLK_x:
                    chip8.keypad[0x0] = isPressed;
                    break;
                case SDLK_c:
                    chip8.keypad[0xB] = isPressed;
                    break;
                case SDLK_v:
                    chip8.keypad[0xF] = isPressed;
                    break;
                }
            }
        }

        chip8.Cycle();

        uint32_t currentTime = SDL_GetTicks();

        if (currentTime - lastCycleTime >= 16)
        {
            if (chip8.delayTimer > 0)
            {
                chip8.delayTimer--;
            }
            if (chip8.soundTimer > 0)
            {
                if (chip8.soundTimer == 1)
                {
                }
                chip8.soundTimer--;
            }
            lastCycleTime = currentTime;
        }

        RenderDisplay(chip8, renderer);

        frameCount++;
        if (currentTime - lastFpsTime >= 1000) 
        {
            std::string title = "CHIP-8 Emulator | FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(window, title.c_str());
            
            frameCount = 0;
            lastFpsTime = currentTime;
        }

        SDL_Delay(2);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}