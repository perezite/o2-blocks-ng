#include <iostream>
#include <SDL3/SDL.h>
#include "LibSdlPlaytests.h"
using namespace std;

namespace o2 
{
	namespace my 
	{
		void LibSdlPlaytests::simpleWindow()
		{
            cout << "Expected behaviour:" << endl;
            cout << "- A simple window, filled with a teal background color, is displayed" << endl;

            SDL_Init(SDL_INIT_VIDEO);
            auto* window = SDL_CreateWindow("Simple window", 800, 600, SDL_WINDOW_RESIZABLE);
            auto* renderer = SDL_CreateRenderer(window, nullptr);
            auto running = true;
            SDL_Event event;

            while (running) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                        running = false;
                }

                SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
                SDL_RenderClear(renderer);
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
            }

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
		}
	}
}