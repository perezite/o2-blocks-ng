#pragma once
#include "../Core/SdlContext.h"
#include "../Core/StlHelper.h"
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
		std::vector<TreeNode*> children = {};
		std::function<void()> action = nullptr;
		inline bool isLeaf() const { return children.empty(); }
		TreeNode(const std::string& text_) : text(text_) { }
		TreeNode(const std::string& text_, std::function<void()> action_) : text(text_), action(action_) {}
		inline ~TreeNode() { deleteAll(children); }	
	};

	class TestMenuWindow
	{
		SdlContext _sdlContext;
		SDL_Window* _window = nullptr;
		SDL_WindowID _windowId;
		SDL_Renderer* _renderer = nullptr;
		ImGuiContext* _imGuiContext = nullptr;
		TreeNode _playtestTree{"Playtests"};
		TreeNode _autotestTree{ "Auto Tests" };
		bool _isOpen = true;
		void drawTree(TreeNode& node, bool isAutotestTree);
	public:
		TestMenuWindow();
		~TestMenuWindow();
		void addPlaytest(const std::string& path, const std::function<void()>& action);
		void addAutotest(const std::string& path, const std::function<void()>& action);
		void update();
		void display();
		const inline bool isOpen() const { return _isOpen; }

		//void printTree();
	};
}