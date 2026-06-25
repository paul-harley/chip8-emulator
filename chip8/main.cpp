#include "Chip8.h"


void testWindow() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL3 Test", 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

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



int main()
{
    Chip8 chip8;

    chip8.loadFont();
    chip8.dumpMemory(0x050, 0x09F);

    chip8.run();

    return 0;
}
