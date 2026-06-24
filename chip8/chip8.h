#pragma once

#include <array>
#include <cstdint>

class Chip8
{
private:
    std::array<uint8_t, 4096> memory{};
    uint16_t pc;

public:
    Chip8();

    void loadFont();
    void dumpMemory(uint8_t start, uint8_t end);
};