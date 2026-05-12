#pragma once
#include "../Core/SdlContext.h"
#include "../Core/Color.h"
#include <SDL3/SDL.h>
#include <string>
#include <cstdint>

namespace o2
{
	class Window
	{
		static SDL_WindowID GlCurrentWindowId;
		SdlContext _sdlContext;
		SDL_Window* _sdlWindow = {};
		SDL_WindowID _windowId;
		SDL_GLContext _glContext;
		Color _clearColor;
		bool _mustClear = false;
		int _width, _height;
		bool _isOpen = true;

		inline bool isCurrent() const { return GlCurrentWindowId == _windowId; }
		void makeCurrent() const;
	public:
		Window(int width, int height, const std::string& title);
		~Window();
		const inline bool isOpen() const { return _isOpen;}
		void update();
		void clear(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);
		void display();
	};
}