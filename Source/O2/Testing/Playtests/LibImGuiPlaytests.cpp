#include "LibImGuiPlaytests.h"
#include "../../Window/Window.h"
#include "../../Core/Events.h"
#include "../../Core/SdlContext.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
#include <functional>
using namespace std;

namespace o2 
{
	namespace my
	{
        struct TreeNode
        {
            string text;

            bool isLeaf = false;

            vector<TreeNode> children;

            function<void()> action;
        };

        void executeDummyAction(const string& name)
        {
            SDL_Log("Action executed for: %s", name.c_str());
        }

        void drawTreeNode(TreeNode& node)
        {
            // -----------------------------
            // Blatt
            // -----------------------------
            if (node.isLeaf)
            {
                ImGui::PushID(&node);

                // Kleiner Action-Button
                if (ImGui::SmallButton(">"))
                {
                    if (node.action)
                        node.action();
                }

                ImGui::SameLine();

                ImGui::Selectable(node.text.c_str());

                if (ImGui::IsItemHovered() &&
                    ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    if (node.action)
                        node.action();
                }

                ImGui::PopID();

                return;
            }

            // -----------------------------
            // Ordner / Gruppe
            // -----------------------------
            if (ImGui::TreeNode(node.text.c_str()))
            {
                for (auto& child : node.children)
                    drawTreeNode(child);

                ImGui::TreePop();
            }
        }

        void LibImGuiPlaytests::drawTree()
        {
            //
            // -----------------------------
            // SDL initialisieren
            // -----------------------------
            //

            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                SDL_Log("SDL_Init failed: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // Fenster
            // -----------------------------
            //

            float mainDisplayScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
            SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            SDL_Window* window = SDL_CreateWindow("Simple ImGUI", (int)(400 * mainDisplayScale), (int)(800 * mainDisplayScale), windowFlags);

            if (!window)
            {
                SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // Renderer
            // -----------------------------
            //

            SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

            if (!renderer)
            {
                SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // ImGui initialisieren
            // -----------------------------
            //

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(mainDisplayScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = mainDisplayScale;        // Set initial font scale. 
            ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);

            //
            // -----------------------------
            // Demo-Daten
            // -----------------------------
            //

            TreeNode root =
            {
                "Root",
                false,
                {
                    {
                        "Characters",
                        false,
                        {
                            {
                                "Player",
                                true,
                                {},
                                []()
                                {
                                    executeDummyAction("Player");
                                }
                            },
                            {
                                "Enemy",
                                true,
                                {},
                                []()
                                {
                                    executeDummyAction("Enemy");
                                }
                            }
                        }
                    },

                    {
                        "Items",
                        false,
                        {
                            {
                                "Sword",
                                true,
                                {},
                                []()
                                {
                                    executeDummyAction("Sword");
                                }
                            },
                            {
                                "Potion",
                                true,
                                {},
                                []()
                                {
                                    executeDummyAction("Potion");
                                }
                            }
                        }
                    }
                }
            };

            //
            // -----------------------------
            // Main Loop
            // -----------------------------
            //

            bool running = true;

            while (running)
            {
                //
                // -----------------------------
                // Events
                // -----------------------------
                //

                SDL_Event event;

                while (SDL_PollEvent(&event))
                {
                    ImGui_ImplSDL3_ProcessEvent(&event);

                    if (event.type == SDL_EVENT_QUIT)
                        running = false;

                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                        running = false;
                }

                //
                // -----------------------------
                // ImGui Frame starten
                // -----------------------------
                //

                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();

                ImGui::NewFrame();

                //
                // -----------------------------
                // GUI
                // -----------------------------
                //

                ImGuiViewport* viewport = ImGui::GetMainViewport();

                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

                ImGuiWindowFlags flags =
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoTitleBar;

                ImGui::Begin("Hierarchy", nullptr, flags);

                drawTreeNode(root);

                ImGui::End();

                ImGui::PopStyleVar(2);

                //
                // -----------------------------
                // Rendern
                // -----------------------------
                //

                ImGui::Render();

                SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);

                SDL_RenderClear(renderer);

                ImGui_ImplSDLRenderer3_RenderDrawData(
                    ImGui::GetDrawData(),
                    renderer
                );

                SDL_RenderPresent(renderer);
            }

            //
            // -----------------------------
            // Cleanup
            // -----------------------------
            //

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

            // SDL initialisieren
            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                SDL_Log("SDL_Init failed: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // ImGui / SDL_Renderer Fenster
            // -----------------------------
            //

            SDL_Window* imguiWindow = SDL_CreateWindow(
                "ImGui Window",
                800,
                600,
                0
            );

            if (!imguiWindow)
            {
                SDL_Log("Failed to create ImGui window: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            SDL_Renderer* renderer = SDL_CreateRenderer(imguiWindow, nullptr);

            if (!renderer)
            {
                SDL_Log("Failed to create renderer: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // OpenGL Fenster
            // -----------------------------
            //

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                SDL_GL_CONTEXT_PROFILE_CORE);

            SDL_Window* glWindow = SDL_CreateWindow(
                "OpenGL Window",
                800,
                600,
                SDL_WINDOW_OPENGL
            );

            if (!glWindow)
            {
                SDL_Log("Failed to create GL window: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            SDL_GLContext glContext = SDL_GL_CreateContext(glWindow);

            if (!glContext)
            {
                SDL_Log("Failed to create GL context: %s", SDL_GetError());
                throw runtime_error("fail");
            }

            //
            // -----------------------------
            // ImGui initialisieren
            // -----------------------------
            //

            IMGUI_CHECKVERSION();

            ImGui::CreateContext();

            ImGui::StyleColorsDark();

            ImGui_ImplSDL3_InitForSDLRenderer(imguiWindow, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);

            //
            // -----------------------------
            // Main Loop
            // -----------------------------
            //

            bool running = true;

            while (running)
            {
                SDL_Event event;

                while (SDL_PollEvent(&event))
                {
                    // ImGui Events
                    ImGui_ImplSDL3_ProcessEvent(&event);

                    if (event.type == SDL_EVENT_QUIT)
                        running = false;

                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                        running = false;
                }

                //
                // -----------------------------
                // ImGui Frame
                // -----------------------------
                //

                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();

                ImGui::NewFrame();

                ImGui::Begin("Hello");

                ImGui::Text("This is SDL_Renderer3 + ImGui");

                static float value = 0.5f;

                ImGui::SliderFloat("Value", &value, 0.0f, 1.0f);

                ImGui::End();

                ImGui::Render();

                //
                // -----------------------------
                // ImGui Fenster rendern
                // -----------------------------
                //

                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

                SDL_RenderClear(renderer);

                ImGui_ImplSDLRenderer3_RenderDrawData(
                    ImGui::GetDrawData(),
                    renderer
                );

                SDL_RenderPresent(renderer);

                //
                // -----------------------------
                // OpenGL Fenster rendern
                // -----------------------------
                //

                SDL_GL_MakeCurrent(glWindow, glContext);

                glViewport(0, 0, 800, 600);

                glClearColor(0.2f, 0.1f, 0.7f, 1.0f);

                glClear(GL_COLOR_BUFFER_BIT);

                SDL_GL_SwapWindow(glWindow);
            }

            //
            // -----------------------------
            // Cleanup
            // -----------------------------
            //

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