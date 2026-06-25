#pragma once

#include <array>
#include <stack>
#include <cstdint>
#include <SDL3/SDL.h>


class Chip8 {

private:
    std::array<uint8_t, 4096> memory{};
    uint16_t pc;

    SDL_Window* window;
    SDL_Renderer* renderer;


    std::stack<uint16_t> stack;
    
    uint8_t delayTimer;
    uint8_t soundTimer;


    void updateWindow();
    uint16_t fetch(); //instruction is 2 bytes


public:
    Chip8();

    void loadFont();
    void dumpMemory(uint16_t start, uint16_t end);
    void run();
    void updateTimers();
};