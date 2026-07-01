#include <iostream>
#include <SDL2/SDL.h>
#include "CHIP8.hpp"

const int scale = 15;
const int SCREEN_WIDTH = 64 * scale;
const int SCREEN_HEIGHT = 32 * scale;

void RenderDisplay(Chip8& chip8, SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
    SDL_RenderClear(renderer);

    for(int y = 0; y < 32; ++y){
        for(int x = 0; x < 64; ++x){
            SDL_Rect pixelRect;
            pixelRect.x = x * scale;
            pixelRect.y = y * scale;
            pixelRect.w = scale - 1;
            pixelRect.h = scale - 1;

            if(chip8.display[(y * 64) + x] == 0xFFFFFFFF){
                SDL_SetRenderDrawColor(renderer,0x2f,  0xc9, 0x00, 0xff);
            }
            else{
                SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xff);
            }

            SDL_RenderFillRect(renderer, &pixelRect);
        }
    }
    SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]){

    if(argc < 2){
        std::cerr << "Error! not enough argument, put some roms bro";
        return 1;
    }
    char const* romFile = argv[1];
 if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Chip8 chip8;
    chip8.LoadROM(romFile);

    bool quit = false;
    SDL_Event e;

    while(!quit){
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
        }
    }

    chip8.Cycle();
    RenderDisplay(chip8, renderer);

    SDL_Delay(2);

    
 }

SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}