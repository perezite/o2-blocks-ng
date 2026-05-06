find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)
add_executable (Blocks ${SOURCES} )
target_link_libraries(Blocks PRIVATE SDL3::SDL3 OpenGL::GL)