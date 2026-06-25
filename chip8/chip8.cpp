#include <iostream>
#include <string>
#include "chip8.h"

Chip8::Chip8() {
	pc = 0x200;

	//	Init Display
	SDL_Init(SDL_INIT_VIDEO);
	
	int WINDOW_HEIGHT = 32;
	int WINDOW_WIDTH = 64;
	int SCALE = 10;

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