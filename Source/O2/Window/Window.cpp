#include "Window.h"
#include "../Core/Events.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
using namespace std;

namespace o2 
{
	Window::Window(int width, int height, const string& title)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		_sdlWindow = SDL_CreateWindow(title.c_str(), 800, 600, SDL_WINDOW_OPENGL);
		if (!_sdlWindow) throw "Failed to create window: %s\n", SDL_GetError();

		_glContext = SDL_GL_CreateContext(_sdlWindow);
		if (!_glContext) throw "Failed to create OpenGL context: %s\n", SDL_GetError();
	}

	Window::~Window()
	{
		SDL_GL_DestroyContext(_glContext);
		SDL_DestroyWindow(_sdlWindow);
	}

	void Window::update()
	{
		auto windowId = SDL_GetWindowID(_sdlWindow);
		if (Events::isWindowCloseRequested(windowId))
			_isOpen = false;
	}

	void Window::clear()
	{
		glClearColor(0.2f, 0.8f, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::display()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}
}
