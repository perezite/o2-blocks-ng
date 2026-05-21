#pragma once
#include "../Core/SdlContext.h"
#include "../Helpers/VectorHelper.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <string>
#include <functional>
#include <vector>

namespace o2
{
	struct Node
	{
		enum class State { Running, Pending, Failed, Succeeded, None };
		std::string text;
		std::vector<Node*> children = {};
		std::function<void()> action = nullptr;
		State state = State::None;
		inline bool isLeaf() const { return children.empty(); }
		inline Node(const std::string& text_) : text(text_) { }
		inline Node(const std::string& text_, std::function<void()> action_) 
			: text(text_), action(action_) { }
		inline ~Node() { deleteAll(children); }
	};

	class TestMenuWindow
	{
		SdlContext _sdlContext;
		SDL_Window* _window = nullptr;
		SDL_WindowID _windowId;
		SDL_Renderer* _renderer = nullptr;
		ImGuiContext* _imGuiContext = nullptr;
		Node _playtestTree{ "Playtests" };
		Node _autotestTree{ "Auto Tests" };
		bool _isOpen = true;
	public:
		TestMenuWindow();
		~TestMenuWindow();
		void addPlaytest(const std::string& path, const std::function<void()>& action);
		void addAutotest(const std::string& path, const std::function<void()>& action);
		void update();
		void display();
		const inline bool isOpen() const { return _isOpen; }
	};
}