#include "LibImGuiPlaytests.h"
#include "../../Window/Window.h"
#include "../../Core/Events.h"
#include "../../Core/SdlContext.h"
#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <iostream>
using namespace std;

namespace o2 
{
	namespace my
	{
		void LibImGuiPlaytests::simpleGui()
		{
            cout << "Expected behaviour: " << endl 
                 << "- A simple gui displayed." << endl
			     << "Technical details: " << endl
			     << "- The rendering uses ImGUI internally" << endl;

            sdlCheck(SDL_Init(SDL_INIT_VIDEO));
            SDL_Window* window = SDL_CreateWindow("Simple ImGUI", 1280, 720, 0);
            sdlCheck(window);
            SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
            sdlCheck(renderer);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();
            float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
            ImGuiStyle& style = ImGui::GetStyle();
            style.ScaleAllSizes(mainScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = mainScale;        // Set initial font scale. 
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