#include "Window.h"
#include "../Core/Events.h"
#include "../Core/SdlContext.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <stdexcept>
using namespace std;

namespace o2 
{
	SDL_WindowID Window::GlCurrentWindowId = 0;

	void Window::makeCurrent() const 
	{
		if (isCurrent()) return;
		GlCurrentWindowId = _windowId;
		SDL_GL_MakeCurrent(_sdlWindow, _glContext);
	}

	Window::Window(int width, int height, const string& title) 
		: _width(width), _height(height)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		_sdlWindow = SDL_CreateWindow(title.c_str(), _width, _height, SDL_WINDOW_OPENGL);
		sdlCheck(_sdlWindow != nullptr);
		_glContext = SDL_GL_CreateContext(_sdlWindow);
		sdlCheck(_glContext != nullptr);

		makeCurrent();
		_windowId = SDL_GetWindowID(_sdlWindow);
		sdlCheck(_windowId);
		sdlCheck(SDL_GL_SetSwapInterval(1));
	}

	Window::~Window()
	{
		sdlCheck(SDL_GL_DestroyContext(_glContext));
		SDL_DestroyWindow(_sdlWindow);
	}

	void Window::update()
	{
		if (Events::isWindowCloseRequested(_windowId)) {
			_isOpen = false;
			sdlCheck(SDL_GL_DestroyContext(_glContext));
			SDL_DestroyWindow(_sdlWindow);
		}
	}

	void Window::clear(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		_clearColor = Color(r, g, b, a);
		_mustClear = true;
	}

	void Window::display()
	{
		if (!isOpen())
			return;

		makeCurrent();
		if (_mustClear) {
			glViewport(0, 0, _width, _height);
			glClearColor(_clearColor.r / 255.0f, _clearColor.g / 255.0f, _clearColor.b / 255.0f, _clearColor.a / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			_mustClear = false;
		}

		SDL_GL_SwapWindow(_sdlWindow);
	}
}
