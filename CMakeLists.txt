cmake_minimum_required(VERSION 3.2.0)
set(PROJECT Sorting)
project(${PROJECT} VERSION 0.1.0)
find_package(SDL2 REQUIRED)
include(CTest)
enable_testing()

add_subdirectory(libs/imgui)

file(GLOB SOURCE_FILES 
src/*.cpp 
src/sort/*.cpp
src/renderer/*.cpp  
)
add_executable(${PROJECT} ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE 
libs/imgui
libs/imgui/backends
includes/
)

target_link_libraries(${PROJECT} SDL2 SDL2main SDL2_ttf imgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
