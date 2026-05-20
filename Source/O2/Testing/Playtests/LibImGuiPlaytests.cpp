#include "LibImGuiPlaytests.h"
#include "../../Window/Window.h"
#include "../../Core/Events.h"
#include "../../Core/SdlContext.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <functional>
using namespace std;

namespace o2 
{
	namespace my
	{
        struct Node
        {
            string text;
            bool isLeaf = false;
            vector<Node> children;
            function<void()> action;
        };

        static void drawTreeNode(Node& node)
        {
            if (node.isLeaf) {
                ImGui::PushID(&node);

                if (ImGui::SmallButton(">")) {
                    if (node.action)
                        node.action();
                }

                ImGui::SameLine();
                ImGui::Selectable(node.text.c_str());

                if (ImGui::IsItemHovered() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
                    || ImGui::IsKeyPressed(ImGuiKey_Enter))) {
                    if (node.action)
                        node.action();
                }

                ImGui::PopID();
                return;
            }

            if (node.text == "Root" || node.text == "Items")
                ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            if (ImGui::TreeNode(node.text.c_str())) {
                for (auto& child : node.children)
                    drawTreeNode(child);

                ImGui::TreePop();
            }
        }

        void LibImGuiPlaytests::treeView()
        {
            sdlCheck(SDL_Init(SDL_INIT_VIDEO));

            float mainDisplayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
            SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            SDL_Window* window = SDL_CreateWindow("Simple ImGUI", (int)(400 * mainDisplayScale), (int)(800 * mainDisplayScale), windowFlags);
            sdlCheck(window);

            SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
            sdlCheck(renderer);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui::StyleColorsDark();
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(mainDisplayScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = mainDisplayScale;        // Set initial font scale. 
            ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);

            Node root = 
            {
                "Root",
                false,
                {
                    {
                        "Characters",
                        false,
                        {
                            { "Player", true, {}, []() { SDL_Log("Action executed for: Player"); }},
                            { "Enemy", true, {}, []() { SDL_Log("Action executed for: Enemy"); }}
                        }
                    },
                    {
                        "Items",
                        false,
                        {
                            { "Sword", true, {}, []() { SDL_Log("Action executed for: Sword"); } },
                            { "Potion", true, {}, []() { SDL_Log("Action executed for: Potion"); }}
                        }
                    }
                }
            };

            bool running = true;

            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL3_ProcessEvent(&event);
                    if (event.type == SDL_EVENT_QUIT)
                        running = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                        running = false;
                }

                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();
                ImGui::NewFrame();

                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

                ImGui::Begin("Hierarchy", nullptr, flags);
                drawTreeNode(root);
                ImGui::End();

                ImGui::PopStyleVar(2);
                ImGui::Render();

                SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
                SDL_RenderClear(renderer);
                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
                SDL_RenderPresent(renderer);
            }

            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        void LibImGuiPlaytests::glWindowAndImGuiWindow()
        {
            cout << "Expected behaviour: " << endl
                << "- Two windows are displayed. One contains a simple GUI, the other contains a flat background color." << endl
                << "Technical details: " << endl
                << "- The GUI window uses ImGui with SDL_Renderer3" << endl
                << "- The background colored window uses SDL3 with OpenGL" << endl;

            sdlCheck(SDL_Init(SDL_INIT_VIDEO));

            SDL_Window* imguiWindow = SDL_CreateWindow("ImGui Window", 800, 600, 0);
            sdlCheck(imguiWindow);
            SDL_Renderer* renderer = SDL_CreateRenderer(imguiWindow, nullptr);
            sdlCheck(renderer);

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_Window* glWindow = SDL_CreateWindow("OpenGL Window", 800, 600, SDL_WINDOW_OPENGL);
            sdlCheck(glWindow);
            SDL_GLContext glContext = SDL_GL_CreateContext(glWindow);
            sdlCheck(glContext);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            ImGui_ImplSDL3_InitForSDLRenderer(imguiWindow, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);

            bool running = true;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL3_ProcessEvent(&event);
                    if (event.type == SDL_EVENT_QUIT)
                        running = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                        running = false;
                }

                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();
                ImGui::NewFrame();
                ImGui::Begin("Hello");
                ImGui::Text("This is SDL_Renderer3 + ImGui");
                static float value = 0.5f;
                ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
                ImGui::End();
                ImGui::Render();

                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                SDL_RenderClear(renderer);
                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
                SDL_RenderPresent(renderer);

                SDL_GL_MakeCurrent(glWindow, glContext);
                glViewport(0, 0, 800, 600);
                glClearColor(0.2f, 0.1f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                SDL_GL_SwapWindow(glWindow);
            }

            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            SDL_GL_DestroyContext(glContext);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(imguiWindow);
            SDL_DestroyWindow(glWindow);
            SDL_Quit();
        }

        void LibImGuiPlaytests::simpleGui()
		{
            cout << "Expected behaviour: " << endl 
                 << "- A simple gui displayed." << endl
			     << "Technical details: " << endl
			     << "- The rendering uses ImGUI internally" << endl;

            sdlCheck(SDL_Init(SDL_INIT_VIDEO));
            float mainDisplayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
            SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            SDL_Window* window = SDL_CreateWindow("Simple ImGUI", (int)(1280 * mainDisplayScale), (int)(800 * mainDisplayScale), windowFlags);
            sdlCheck(window);
            SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
            sdlCheck(renderer);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            ImGui::StyleColorsDark();
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(mainDisplayScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = mainDisplayScale;        // Set initial font scale. 
            ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);

            bool running = true;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL3_ProcessEvent(&event);

                    if (event.type == SDL_EVENT_QUIT) running = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) running = false;
                }

                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();
                
                ImGui::NewFrame();
                ImGui::Begin("Hello");
                ImGui::Text("Hello from ImGui!");
                static float value = 0.5f;
                ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);
                if (ImGui::Button("Quit"))running = false;
                ImGui::End();

                ImGui::Render();

                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
                SDL_RenderClear(renderer);

                ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

                SDL_RenderPresent(renderer);
            }

            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
		}
	}
}