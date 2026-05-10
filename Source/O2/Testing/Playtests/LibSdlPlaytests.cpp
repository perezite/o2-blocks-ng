#include "LibSdlPlaytests.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <tuple>
#include <exception>
#include <stdexcept>
using namespace std;

namespace o2  
{
	namespace my 
    {
        struct SdlWindow 
        {
            int width, height;
            SDL_Window* window = nullptr;
            SDL_GLContext context = nullptr;
            SDL_WindowID windowId = 0;
            const bool isOpen() const { return window != nullptr; }
        };

        inline static void sdlCheck(const bool okCondition) 
        {
            if (!okCondition)
                throw runtime_error(SDL_GetError());
        }

        static void destroyGlWindow(SdlWindow& sdlWindow) 
        {
            if (!sdlWindow.isOpen()) return;
            SDL_GL_DestroyContext(sdlWindow.context);
            sdlWindow.context = nullptr;
            SDL_DestroyWindow(sdlWindow.window);
            sdlWindow.window = nullptr;
        }

        static void clearAndSwapGlWindow(const SdlWindow& sdlWindow, GLclampf r, GLclampf g, GLclampf b, GLclampf alpha) 
        {
            SDL_GL_MakeCurrent(sdlWindow.window, sdlWindow.context);
            glViewport(0, 0, sdlWindow.width, sdlWindow.height);
            glClearColor(r, g, b, alpha);
            glClear(GL_COLOR_BUFFER_BIT);
            SDL_GL_SwapWindow(sdlWindow.window);
        }

        static SdlWindow createGlWindow(int width, int height, const string& title) 
        {
            auto window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
            sdlCheck(window);
            auto context = SDL_GL_CreateContext(window);
            sdlCheck(context);
            sdlCheck(SDL_GL_MakeCurrent(window, context));
            auto windowId = SDL_GetWindowID(window);
            sdlCheck(windowId);
            sdlCheck(SDL_GL_SetSwapInterval(1));

            return SdlWindow{ width, height, window, context, windowId };
        }

        void LibSdlPlaytests::twoOpenGlWindows() 
        {
            cout << "Expected behaviour:" << endl;
            cout << "- Two windows, filled with different background colors and sizes, are displayed" << endl;
            cout << "- The background colors are rendered using OpenGL, internally" << endl;

            if (!SDL_Init(SDL_INIT_VIDEO)) throw runtime_error(SDL_GetError());
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            auto firstSdlWindow = createGlWindow(800, 600, "First window with OpenGL");
            auto secondSdlWindow = createGlWindow(400, 300, "Second window with OpenGL");

            SDL_Event event;
            while (firstSdlWindow.isOpen() || secondSdlWindow.isOpen()) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                        if (event.window.windowID == firstSdlWindow.windowId) 
                            destroyGlWindow(firstSdlWindow);
                        else if (event.window.windowID == secondSdlWindow.windowId) 
                            destroyGlWindow(secondSdlWindow); 
                    }
                }
                    
                if (firstSdlWindow.isOpen())
                    clearAndSwapGlWindow(firstSdlWindow, 1, 0, 0, 1);
                if (secondSdlWindow.isOpen())
                    clearAndSwapGlWindow(secondSdlWindow, 0, 0, 1, 1);
            }

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
                
                glClearColor(.1f, .2f, .3f, 1);
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