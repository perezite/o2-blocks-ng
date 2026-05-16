#include "TestMenuWindow.h"
#include "../Core/Events.h"
#include "../Core/StdHelper.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
#include <functional>
using namespace std;

namespace o2
{
    //static void printTheTree(TreeNode& node, size_t depth = 0)
    //{
    //    string indent(2 * depth, ' ');

    //    if (node.isLeaf()) {
    //        if (node.action == nullptr) throw runtime_error("Node has no action");
    //        cout << indent << "> " << node.text << endl;
    //        return;
    //    }
    //    
    //    cout << indent << "- " << node.text << endl;
    //    for (auto& child : node.children)
    //        printTheTree(*child, depth + 1);
    //}

    //void TestMenuWindow::printTree()
    //{
    //    printTheTree(_playtestTree);
    //}

    inline bool isKeyPressed()
    {
        return false;
    }

    template<typename First, typename... Rest>
    static bool isKeyPressed(First first, Rest... rest)
    {
        return ImGui::IsKeyPressed(first)
            || isKeyPressed(rest...);
    }

    static bool isItemSelected() 
    {
        auto selected = ImGui::IsItemHovered() &&
            (ImGui::IsMouseClicked(ImGuiMouseButton_Left)
                || isKeyPressed(ImGuiKey_Enter, ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl));
        return selected;
    }

    static void addTest(TreeNode& testTree, const string& path, const function<void()>& action)
    {
        if (path.empty()) throw runtime_error("Path must not be empty");

        TreeNode* node = &testTree;
        auto pathElements = split(path, "/");

        // Create / find and then add inner nodes
        for (size_t i = 0; i < pathElements.size() - 1; i++) {
            const auto& pathElement = pathElements[i];
            auto* foundChild = singleOrNull(node->children, [&](const TreeNode* child) {
                return child->text == pathElement;
                });

            if (!foundChild) {
                node->children.push_back(new TreeNode(pathElement));
                foundChild = node->children.back();
            }

            node = foundChild;
        }

        // Add leaf node
        const auto& leafName = pathElements.back();
        node->children.push_back(new TreeNode(leafName, action));
    }

    static void getAllLeaves(TreeNode& node, vector<TreeNode*>& allLeaves) {
        if (node.isLeaf()) {
            allLeaves.push_back(&node);
            return;
        }

        for (auto* child : node.children)
            getAllLeaves(*child, allLeaves);
    }

    void TestMenuWindow::drawTree(TreeNode& node, bool isAutotestTree)
    {
        // draw leaf
        if (node.isLeaf()) {
            ImGui::PushID(&node);

            bool selected = ImGui::SmallButton(">");
            if (selected || isItemSelected()) {
                if (node.action) node.action();
            }

            ImGui::SameLine();
            selected = ImGui::Selectable(node.text.c_str());
            if (selected && isItemSelected()) {
                if (node.action) node.action();
            }

            ImGui::PopID();
            return;
        }

        // draw inner node
        bool open = ImGui::TreeNode(node.text.c_str());
        if (isAutotestTree && ImGui::IsItemHovered() && isKeyPressed(ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl)) {
            vector<TreeNode*> selectedTests;
            getAllLeaves(node, selectedTests);
            cout << "Selected tests: " << endl;
            for (auto* selectedTest : selectedTests)
                cout << selectedTest->text << endl;
        }

        if (open) {
            for (auto& child : node.children)
                drawTree(*child, isAutotestTree);
            ImGui::TreePop();
        }
    }

    TestMenuWindow::TestMenuWindow()
	{
        // create SDL window and renderer
        auto mainDisplayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        auto windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        auto primaryDisplayId = SDL_GetPrimaryDisplay();
        sdlCheck(primaryDisplayId);
        SDL_Rect bounds;
        sdlCheck(SDL_GetDisplayUsableBounds(primaryDisplayId, &bounds));
        _window = SDL_CreateWindow("Tests", (int)(400 * mainDisplayScale), (int)(bounds.h * 0.75f), windowFlags);
        sdlCheck(_window);
        _windowId = SDL_GetWindowID(_window); 
        SDL_SetWindowPosition(_window, int(30 * mainDisplayScale), int(40 * mainDisplayScale));
        _renderer = SDL_CreateRenderer(_window, nullptr);
        sdlCheck(_renderer);

        // create ImGui context
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

    TestMenuWindow::~TestMenuWindow()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void TestMenuWindow::addPlaytest(const string& path,const function<void()>& action)
    {
        addTest(_playtestTree, path, action);
    }

    void TestMenuWindow::addAutotest(const std::string& path, const std::function<void()>& action)
    {
        addTest(_autotestTree, path, action);
    }

    void TestMenuWindow::update()
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

        ImGui::Begin("Tests", nullptr, flags);
        if (ImGui::BeginTabBar("Testing Tabs")) {
            if (ImGui::BeginTabItem("Playtests")) {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);   // set root item as open
                drawTree(_playtestTree, false);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Auto Tests")) {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);   // set root item as open
                drawTree(_autotestTree, true);
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::Render();

/*        if (any(_queuedAutotests))
        {
            cout << "Queued autotests: " << endl;
            for (auto* queuedAutotest : _queuedAutotests)
                cout << queuedAutotest->text << endl;
            _queuedAutotests.clear();;
        } */      
    }

    void TestMenuWindow::display()
    {
        SDL_SetRenderDrawColor(_renderer, 35, 35, 35, 255);
        SDL_RenderClear(_renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
        SDL_RenderPresent(_renderer);
    }
}