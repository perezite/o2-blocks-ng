#include "PlaytestMenuWindow.h"
#include "../../Core/Events.h"
#include "../../Core/StdHelper.h"
#include "../../Core/StlHelper.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
#include <functional>
using namespace std;

namespace o2
{
    static void printTheTree(TreeNode& node, size_t depth = 0)
    {
        string indent(2 * depth, ' ');

        if (node.isLeaf()) {
            auto hasAction = node.action != nullptr;
            cout << indent << "> " << node.text << (hasAction ? " (has action)" : " (has NO action)") << endl;
            return;
        }
        
        cout << indent << "- " << node.text << endl;
        for (auto& child : node.children)
            printTheTree(*child, depth + 1);
    }

    void PlaytestMenuWindow::printTree()
    {
        printTheTree(_tree);
    }

	PlaytestMenuWindow::PlaytestMenuWindow()
	{
        auto mainDisplayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        auto windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        _window = SDL_CreateWindow("Playtests", (int)(400 * mainDisplayScale), (int)(800 * mainDisplayScale), windowFlags);
        sdlCheck(_window);
        _windowId = SDL_GetWindowID(_window);
        _renderer = SDL_CreateRenderer(_window, nullptr);
        sdlCheck(_renderer);

        IMGUI_CHECKVERSION();
        _imGuiContext = ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(mainDisplayScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = mainDisplayScale;        // Set initial font scale. 
        ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer);
        ImGui_ImplSDLRenderer3_Init(_renderer);
	}

    PlaytestMenuWindow::~PlaytestMenuWindow()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void PlaytestMenuWindow::addPlaytest(const string& path,const function<void()>& action)
    {
        if (path.empty())
            throw runtime_error("Path must not be empty");

        TreeNode* node = &_tree;
        auto pathElements = split(path, "/");

        // Create/find all inner nodes
        for (size_t i = 0; i < pathElements.size() - 1; i++){
            const auto& pathElement = pathElements[i];
            auto* foundNode = singleOrNull(node->children, [&](const TreeNode& child) {
                return child.text == pathElement;
            });

            if (!foundNode) {
                auto newNode = make_unique<TreeNode>(pathElement);
                node->children.emplace_back(move(newNode));
                foundNode = node->children.back().get();
            }

            node = foundNode;
        }

        // Add leaf node
        const auto& leafName = pathElements.back();
        auto leaf = make_unique<TreeNode>(leafName, action);
        node->children.emplace_back(move(leaf));
    }

    void PlaytestMenuWindow::update()
    {
        if (Events::isWindowCloseRequested(_windowId))
            _isOpen = false;

        for (auto& event : Events::getSdlEvents())
            ImGui_ImplSDL3_ProcessEvent(&event);

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Hello");
        ImGui::Text("This is SDL_Renderer3 + ImGui");
        static float value = 0.5f;
        ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::Render();
    }

    void PlaytestMenuWindow::display()
    {
        SDL_SetRenderDrawColor(_renderer, 35, 35, 35, 255);
        SDL_RenderClear(_renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
        SDL_RenderPresent(_renderer);
    }
}