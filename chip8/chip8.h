#pragma once

#include <array>
#include <stack>
#include <cstdint>
#include <tuple>
#include <string>
#include <SDL3/SDL.h>


class Chip8 {

private:
    std::array<uint8_t, 4096> memory{};
    std::array<uint8_t, 16> registers{};

    static constexpr uint8_t WINDOW_WIDTH = 64;
    static constexpr uint8_t WINDOW_HEIGHT = 32;

    std::array<uint8_t, WINDOW_WIDTH * WINDOW_HEIGHT > display;

    uint16_t indexReg;

    uint16_t pc;

    SDL_Window* window;
    SDL_Renderer* renderer;


    std::stack<uint16_t> stack;
    
    uint8_t delayTimer;
    uint8_t soundTimer;


    void updateWindow();
    uint16_t fetch(); //instruction is 2 bytes
    void decode(uint16_t instruction);

    void clearScreen();
    void jump(uint16_t jump_loc);
    void setReg(uint8_t reg, uint8_t value);
    void addToReg(int8_t reg, uint8_t value);
    void setIndex(uint16_t value);
    void draw(uint8_t x, uint8_t y, uint8_t n);


public:
    int SCALE = 10;

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> OFF_COLOUR = std::make_tuple(0, 0, 0, 255);
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> ON_COLOUR = std::make_tuple(255, 255, 255, 255);

    Chip8();

    void loadFont();
    void dumpMemory(uint16_t start, uint16_t end);
    void run();
    void updateTimers();
    bool loadRom(const std::string filename);
};