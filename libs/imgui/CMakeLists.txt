cmake_minimum_required(VERSION 3.2.0)
set(PROJECT imgui)
project(${PROJECT} VERSION 0.1.0)
find_package(SDL2 REQUIRED)

file(GLOB SOURCE_FILES 
*.cpp 
backends/imgui_impl_sdl2.cpp
backends/imgui_impl_sdlrenderer2.cpp
)
add_library(${PROJECT} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE 
.
backends
)
target_link_libraries(${PROJECT} SDL2 SDL2main)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
