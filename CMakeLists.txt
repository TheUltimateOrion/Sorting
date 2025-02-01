cmake_minimum_required(VERSION 3.30.1)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")
set(PROJECT Sorting)
project(${PROJECT} VERSION 0.1.0)
include(CTest)
enable_testing()

set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies

add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)
add_subdirectory(libs/SDL_ttf EXCLUDE_FROM_ALL)

file(GLOB SOURCE_FILES 
src/imgui/*.cpp 
src/imgui/backends/*.cpp 
src/*.cpp 
src/sort/*.cpp
src/renderer/*.cpp 
)

add_executable(${PROJECT} ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE 
includes/
includes/imgui
includes/imgui/backend
includes/openal
)



target_link_libraries(${PROJECT} PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3 openal)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS ${PROJECT} SDL3-shared SDL3_ttf-shared RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist)
install(DIRECTORY res/ DESTINATION ${CMAKE_SOURCE_DIR}/dist/res)
include(CPack)