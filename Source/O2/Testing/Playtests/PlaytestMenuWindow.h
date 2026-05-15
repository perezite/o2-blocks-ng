#pragma once
#include "../../Core/SdlContext.h"
#include "imgui.h"
#include <SDL3/SDL.h>

namespace o2
{
	class PlaytestMenuWindow
	{
		SdlContext _sdlContext;
		SDL_Window* _window = nullptr;
		SDL_WindowID _windowId;
		SDL_Renderer* _renderer = nullptr;
		ImGuiContext* _imGuiContext = nullptr;
		bool _isOpen = true;
	public:
		PlaytestMenuWindow();
		~PlaytestMenuWindow();
		void update();
		void display();
		const inline bool isOpen() const { return _isOpen; }
	};
}