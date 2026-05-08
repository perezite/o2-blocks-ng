#include "LibSdlPlaytests.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <tuple>
using namespace std;

namespace o2 
{
	namespace my
	{
        tuple<SDL_Window*, SDL_GLContext> createGlWindow(int width, int height, const string& title)
        {
            auto window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
            if (!window) SDL_GetError();
            auto context = SDL_GL_CreateContext(window);
            if (!context) SDL_GetError();

            return {window, context};
        }

        void clearAndSwapGlWindow(SDL_Window* window, SDL_GLContext context, 
            GLclampf r, GLclampf g, GLclampf b, GLclampf alpha)
        {
            SDL_GL_MakeCurrent(window, context);
            glClearColor(r, g, b, alpha);
            glClear(GL_COLOR_BUFFER_BIT);
            SDL_GL_SwapWindow(window);
        }

        void destroyGlWindow(SDL_Window* window, SDL_GLContext context)
        {
            SDL_GL_DestroyContext(context);
            SDL_DestroyWindow(window);
        }

        void LibSdlPlaytests::twoOpenGlWindows()
        {
            cout << "Expected behaviour:" << endl;
            cout << "- Two windows, filled with different background colors and sizes, are displayed" << endl;
            cout << "- The background colors are rendered using OpenGL, internally" << endl;

            if (!SDL_Init(SDL_INIT_VIDEO))
                throw SDL_GetError();
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            auto firstWindow = createGlWindow(800, 600, "First window with OpenGL");
            auto secondWindow = createGlWindow(400, 300, "Second window with OpenGL");

            SDL_GL_SetSwapInterval(1);

            auto running = true;
            SDL_Event event;

            while (running) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                        running = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                        running = false;
                }

                clearAndSwapGlWindow(get<0>(firstWindow), get<1>(firstWindow), 1, 0, 0, 1);
                clearAndSwapGlWindow(get<0>(secondWindow), get<1>(secondWindow), 0, 0, 1, 1);
            }

            destroyGlWindow(get<0>(firstWindow), get<1>(firstWindow));
            destroyGlWindow(get<0>(secondWindow), get<1>(secondWindow));
            SDL_Quit();
        }

        void LibSdlPlaytests::openGlWindow()
        {
            cout << "Expected behaviour:" << endl;
            cout << "- A simple window, filled with a teal background color, is displayed" << endl;
            cout << "- The background color is rendered using OpenGL, internally" << endl;

            if (!SDL_Init(SDL_INIT_VIDEO)) 
                throw "Failed to create SDL video: %s\n", SDL_GetError();
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            auto window = SDL_CreateWindow("Simple window with OpenGL", 800, 600, SDL_WINDOW_OPENGL);
            if (!window) throw "Failed to create window: %s\n", SDL_GetError();

            auto context = SDL_GL_CreateContext(window);
            if (!context) throw "Failed to create OpenGL context: %s\n", SDL_GetError();

            SDL_GL_SetSwapInterval(1);

            auto running = 1;
            SDL_Event event;

            while (running) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT) 
                        running = 0;
                }
                
                glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                SDL_GL_SwapWindow(window);
            }

            SDL_GL_DestroyContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        void LibSdlPlaytests::simpleWindow()
		{
            cout << "Expected behaviour:" << endl;
            cout << "- A simple window, filled with a teal background color, is displayed" << endl;

            SDL_Init(SDL_INIT_VIDEO);
            auto window = SDL_CreateWindow("Simple SDL window", 800, 600, SDL_WINDOW_RESIZABLE);
            auto renderer = SDL_CreateRenderer(window, nullptr);
            auto running = true;
            SDL_Event event;

            while (running) {
                auto count = 0;
                while (SDL_PollEvent(&event)) {
                    count++;
                    if (event.type == SDL_EVENT_QUIT)
                        running = false;
                }
                cout << count << " ";

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