#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "chip8.h"

Chip8::Chip8() {
	pc = 0x200;
	indexReg = 0;

	//	Init Display
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Main", WINDOW_WIDTH * SCALE, WINDOW_HEIGHT * SCALE, 0);
	renderer = SDL_CreateRenderer(window, NULL);

	delayTimer = 0;
	soundTimer = 0;


}

void Chip8::loadFont() {


	uint8_t font[] = {
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

	int startLoc = 80;
	int endLoc = 159;

	for (int i = startLoc; i < endLoc; i++ ) {
		memory[i] = font[i - startLoc];
	}

}


bool Chip8::loadRom(const std::string filename) {

	std::ifstream rom(filename, std::ios::binary);

	if (!rom) {
		std::cout << "ROM not found...\n";
		return false;
	}

	uint8_t byte;
	uint16_t address = 0x200;

	while (rom.read(reinterpret_cast<char*>(&byte), 1)) {
		memory[address++] = byte;
	}


	return true;

}

void Chip8::dumpMemory(uint16_t start, uint16_t end) {
	for (uint16_t i = start; i < end; i++) {
		std::cout
			<< "0x" << std::hex << i << " : "
			<< std::dec << static_cast<int>(memory[i]) << "\n";

	}
}


void Chip8::updateTimers() {
	if (soundTimer > 0)
		soundTimer --;

	if (delayTimer > 0)
		delayTimer --;
}


void Chip8::run() {

	bool running = true;
	SDL_Event event;

	while (running)
	{
		// handle events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;
		}

		// clear to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// draw white square
		SDL_FRect square = { 300.0f, 200.0f, 200.0f, 200.0f };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &square);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

uint16_t Chip8::fetch() {

	uint16_t instruction;
	
	instruction = (memory[pc] << 8 | memory[pc+1]);
	pc += 2;

	return instruction;
}


void Chip8::decode(uint16_t instruction) {

	//X: The second nibble. Used to look up one of the 16 registers(VX) from V0 through VF.
	//Y : The third nibble. Also used to look up one of the 16 registers(VY) from V0 through VF.
	//N : The fourth nibble. A 4 - bit number.
	//NN : The second byte (third and fourth nibbles). An 8 - bit immediate number.
	//NNN : The second, third and fourth nibbles. A 12 - bit immediate memory address.

	uint8_t X = (instruction >> 8) & 0x0F;
	uint8_t Y = (instruction >> 4) & 0x00F;
	uint8_t N = instruction & 0x000F;
	uint8_t NN = instruction & 0x00FF;
	uint16_t NNN = instruction & 0x0FFF;


	uint16_t opcode = instruction & 0xF000;

	// Decoding time 

	switch (opcode) {
		//00E0 (clear screen)
		case 0x00E0:
			clearScreen();
			break;


		//1NNN (jump)
		//6XNN (set register VX)
		//7XNN (add value to register VX)
		//ANNN (set index register I)
		
		case 0xD000:
			draw(registers[X], registers[Y], N);
			break;
	
	
	
	
	}
}



void Chip8::clearScreen() {

	for (uint8_t i = 0; i < WINDOW_WIDTH; i++) {
		for (uint8_t k = 0; k < WINDOW_HEIGHT; k++) {
			display[i, k] = 0;
		}
	}
	
}

void Chip8::jump(uint16_t jump_loc) {
	pc = jump_loc;
}

void Chip8::setReg(uint8_t reg, uint8_t value) {
	registers[reg] = value;
}

void Chip8::addToReg(int8_t reg, uint8_t value) {
	registers[reg] += value;
}


void Chip8::setIndex(uint16_t value) {
	indexReg = value;
}

void Chip8::draw(uint8_t x, uint8_t y, uint8_t n) {

	x = x % WINDOW_WIDTH;
	y = y % WINDOW_WIDTH;

	registers[0xF] = 0;

	for (uint8_t i = 0; i < n; i++) {

		y += n;
		uint8_t x_offset = 0;

		uint8_t spriteData = memory[indexReg + n];

		// mask goes from 1000 0000 -> 0100 0000 -> 0010 0000 -> ...
		for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
			
			x += x_offset;

			if (spriteData & mask) {
				// bit is 1
				if (display[x, y] == 1) {
					display[x, y] = 0;
				}

				else {
					display[x, y] = 1;
				}

			}


			x_offset++;
			if (x > WINDOW_WIDTH || y > WINDOW_HEIGHT) {
				break;
			}

		}
	}

}


