#pragma once
#include "../Core/SdlContext.h"
#include <SDL3/SDL.h>
#include <string>

namespace o2
{
	class Window
	{
		SdlContext _sdlContext;
		SDL_Window* _sdlWindow = nullptr;
		SDL_GLContext _glContext;
		bool _isOpen = true;
	public:
		Window(int width, int height, const std::string& title);
		~Window();
		const inline bool isOpen() const { return _isOpen;}
		void update();
		void clear();
		void display();
	};
}