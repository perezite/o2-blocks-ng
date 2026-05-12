# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Blocks")

# SDL3
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
add_subdirectory(ThirdParty/SDL)

# ImGui
add_library(imgui STATIC
    ThirdParty/imgui/imgui.cpp
    ThirdParty/imgui/imgui_draw.cpp
    ThirdParty/imgui/imgui_tables.cpp
    ThirdParty/imgui/imgui_widgets.cpp

    ThirdParty/imgui/backends/imgui_impl_sdl3.cpp
    ThirdParty/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC
    ThirdParty/imgui
    ThirdParty/imgui/backends
)

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
)

# App
add_subdirectory ("Source")
