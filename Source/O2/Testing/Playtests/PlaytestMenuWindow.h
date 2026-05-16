#pragma once
#include "../../Core/SdlContext.h"
#include "imgui.h"
#include <SDL3/SDL.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace o2
{
	struct TreeNode
	{
		std::string text;
		std::vector<std::unique_ptr<TreeNode>> children = {};
		std::function<void()> action = nullptr;
		inline TreeNode(std::string t) : text(t) { }
		inline TreeNode(std::string t, std::function<void()> a) : text(move(t)), action(move(a)) { }
		inline bool isLeaf() const { return children.empty(); }
	};

	class PlaytestMenuWindow
	{
		SdlContext _sdlContext;
		SDL_Window* _window = nullptr;
		SDL_WindowID _windowId;
		SDL_Renderer* _renderer = nullptr;
		ImGuiContext* _imGuiContext = nullptr;
		TreeNode _tree{"Playtests"};
		bool _isOpen = true;
	public:
		PlaytestMenuWindow();
		~PlaytestMenuWindow();
		void addPlaytest(const std::string& path, const std::function<void()>& action);
		void update();
		void display();
		const inline bool isOpen() const { return _isOpen; }

		void printTree();
	};
}