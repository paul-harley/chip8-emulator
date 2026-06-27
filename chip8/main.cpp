#include "Chip8.h"
#include <iostream>

int main()
{
    Chip8 chip8;

    //chip8.loadFont();
    //chip8.dumpMemory(0x050, 0x09F);

    //chip8.run();


    bool result = chip8.loadRom("roms/IBM Logo.ch8");

    if (result == true) {

        std::cout << "works";
    }
    else {
        std::cout << "bad";
    }

    return 0;
}
