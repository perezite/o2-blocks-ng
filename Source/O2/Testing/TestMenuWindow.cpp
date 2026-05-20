#include "TestMenuWindow.h"
#include "../Core/Events.h"
#include "../Core/Color.h"
#include "../Helpers/StringHelper.h"
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
    //        if (node.action == nullptr) throw runtime_error("Leaf has no action");
    //        cout << indent << "> " << node.text << endl;
    //        return;
    //    }
    //    
    //    cout << indent << "- " << node.text << endl;
    //    for (auto& child : node.children)
    //        printTheTree(*child, depth + 1);
    //}

    inline static bool isKeyPressed(vector<ImGuiKey> keys)
    {
        for (auto key : keys)
            if (ImGui::IsKeyPressed(key)) return true;

        return false;
    }

    static bool isItemSelected() 
    {
        auto someKeyPressed = isKeyPressed({ ImGuiKey_Space, ImGuiKey_Enter, ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl });
        auto isMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
        auto selected = ImGui::IsItemHovered() && (someKeyPressed || isMouseClicked);
        return selected;
    }

    static void addTest(Node& testTree, const string& path, const function<void()>& action)
    {
        if (path.empty()) throw runtime_error("Path must not be empty");

        Node* node = &testTree;
        auto pathElements = split(path, "/");

        // Create / find and then add inner nodes
        for (size_t i = 0; i < pathElements.size() - 1; i++) {
            const auto& pathElement = pathElements[i];
            auto* foundChild = singleOrNull(node->children, [&](const Node* child) {
                return child->text == pathElement;
                });

            if (!foundChild) {
                node->children.push_back(new Node(pathElement));
                foundChild = node->children.back();
            }

            node = foundChild;
        }

        // Add leaf node
        const auto& leafName = pathElements.back();
        node->children.push_back(new Node(leafName, action));
    }

    static void getAllLeaves(Node& node, vector<Node*>& allLeaves) {
        if (node.isLeaf()) {
            allLeaves.push_back(&node);
            return;
        }

        for (auto* child : node.children)
            getAllLeaves(*child, allLeaves);
    }

    const Color getColor(const Node::State& state) {
        switch (state) {
            case Node::State::None:         return Color(255, 255, 255, 255);
            case Node::State::Pending:      return Color(255, 255,   0, 255);
            case Node::State::Running:      return Color(255, 165,   0, 255);
            case Node::State::Failed:       return Color(255,   0,   0, 255);
            case Node::State::Succeeded:    return Color(  0, 255,   0, 255);
            default: throw runtime_error("Not implemented");
        }
    }

    const Node::State getState(const Node& node)
    {
        if (node.isLeaf()) return node.state;

        bool hasPending = false;
        bool hasRunning = false;
        bool hasSucceeded = false;
        bool hasFailed = false;
        bool hasNone = false;
        for (auto& child : node.children) {
            auto childState = getState(*child);
            switch (childState) {
                case Node::State::Running:      hasRunning = true; break;
                case Node::State::Pending:      hasPending = true; break;
                case Node::State::Failed:       hasFailed = true; break;
                case Node::State::Succeeded:    hasSucceeded = true; break;
                case Node::State::None:         hasNone = true; break;
            }
        }

        if (hasRunning) return Node::State::Running;
        if (hasPending) return Node::State::Pending;
        if (hasFailed) return Node::State::Failed;
        if (hasNone) return Node::State::None;
        if (hasSucceeded) return Node::State::Succeeded;
        throw runtime_error("not implemented");
    }

    void runNextAutotest(Node& autotestTree)
    {
        vector<Node*> autotests;
        getAllLeaves(autotestTree, autotests);
        auto nextRunningAutotest = firstOrDefault(autotests, [](const Node* n) {
            return n->state == Node::State::Running;
            });
        auto nextPendingAutotest = firstOrDefault(autotests, [](const Node* n) {
            return n->state == Node::State::Pending;
        });

        // execute running autotest
        if (nextRunningAutotest)
        {
            try {
                nextRunningAutotest->action();
                nextRunningAutotest->state = Node::State::Succeeded;
            }
            catch (const exception&) {
                nextRunningAutotest->state = Node::State::Failed;
            }
        }

        // set pending autotest to running
        if (nextPendingAutotest)
            nextPendingAutotest->state = Node::State::Running;
    }

    void TestMenuWindow::drawTree(Node& node, bool isAutotestTree)
    {
        // draw leaf
        if (node.isLeaf()) {

            ImGui::PushID(&node);
            auto color = getColor(node.state);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
            auto selected = ImGui::Selectable(node.text.c_str());
            ImGui::PopStyleColor();
            ImGui::PopID();

            if (selected || isItemSelected()) {
                if (isAutotestTree)
                    node.state = Node::State::Pending;
                else if (node.action) 
                    node.action();
            }

            return;
        }

        // draw inner node
        auto state = getState(node);
        auto color = getColor(state);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
        bool open = ImGui::Node(node.text.c_str());
        ImGui::PopStyleColor();
        auto someKeyPressed = isKeyPressed({ ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl });
        if (isAutotestTree && ImGui::IsItemHovered() && someKeyPressed) {
            vector<Node*> leaves;
            getAllLeaves(node, leaves);
            //cout << "Selected tests: " << endl;
            for (auto* leave : leaves)
                leave->state = Node::State::Pending;
                //cout << selectedTest->text << endl;
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
        // process events
        if (Events::isWindowCloseRequested(_windowId))
            _isOpen = false;
        for (auto& event : Events::getSdlEvents())
            ImGui_ImplSDL3_ProcessEvent(&event);

        // run autotest
        runNextAutotest(_autotestTree);

        // set styles
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

        // draw
        ImGui::Begin("Tests", nullptr, flags);
        if (ImGui::BeginTabBar("Testing Tabs")) {
            if (ImGui::BeginTabItem("Playtests")) {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);   // set root item as open initially
                drawTree(_playtestTree, false);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Auto Tests")) {
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);   // set root item as open initially
                drawTree(_autotestTree, true);
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }

        // render
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::Render();
    }

    void TestMenuWindow::display()
    {
        SDL_SetRenderDrawColor(_renderer, 35, 35, 35, 255);
        SDL_RenderClear(_renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
        SDL_RenderPresent(_renderer);
    }
}