if(ANDROID)
    set(OBOE_SOURCE "${CMAKE_SOURCE_DIR}/external/oboe" CACHE PATH "Oboe source root" FORCE)
    set(ALSOFT_BACKEND_OBOE ON CACHE BOOL "" FORCE)
    set(ALSOFT_INSTALL  OFF CACHE BOOL "" FORCE)
endif()

set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS    OFF CACHE BOOL "" FORCE)
set(ALSOFT_UTILS    OFF CACHE BOOL "" FORCE)

add_subdirectory(openal-soft EXCLUDE_FROM_ALL)
add_subdirectory(SDL)

set(SDLTTF_VENDORED ON)  # SDL_ttf builds its own dependencies
add_subdirectory(SDL_ttf)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)

target_link_libraries(ImGui PUBLIC SDL3::SDL3)  # plus OpenGL if you use it
add_library(ImGui::ImGui ALIAS ImGui)

