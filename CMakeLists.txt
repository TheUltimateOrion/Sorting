cmake_minimum_required(VERSION 3.30.1)
set(PROJECT Sorting)
project(${PROJECT} VERSION 0.1.0)
include(CPack)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_subdirectory(external)

file(GLOB SOURCE_FILES 
src/imgui/*.cpp 
src/imgui/backends/*.cpp 
src/*.cpp 
src/core/*.cpp
src/sort/*.cpp
src/renderer/*.cpp 
src/sound/*.cpp 
src/utils/*.cpp 
)

add_executable(${PROJECT} ${SOURCE_FILES})

target_include_directories(${PROJECT} PRIVATE 
includes/
includes/imgui
includes/imgui/backend
includes/openal
)

if(WIN32)
    set(OPENAL_LIB openal)
    install(TARGETS ${PROJECT} SDL3-shared SDL3_ttf-shared RUNTIME DESTINATION .)
elseif(APPLE)
    set(OPENAL_LIB "-framework OpenAL")
    install(TARGETS ${PROJECT} RUNTIME DESTINATION .)
    install(TARGETS SDL3-shared SDL3_ttf-shared LIBRARY DESTINATION .)
    set_target_properties(${PROJECT} PROPERTIES
        BUILD_RPATH "@executable_path"
        INSTALL_RPATH "@executable_path"
    )
elseif(LINUX)
    set(OPENAL_LIB openal)
    install(TARGETS ${PROJECT} RUNTIME DESTINATION .)
    install(TARGETS SDL3-shared SDL3_ttf-shared LIBRARY DESTINATION .)
    set_target_properties(${PROJECT} PROPERTIES
        BUILD_RPATH "$ORIGIN"
        INSTALL_RPATH "$ORIGIN"
    )
endif()

target_link_libraries(${PROJECT} PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3 ${OPENAL_LIB})
install(DIRECTORY res/ DESTINATION res)

include(GoogleTest)

file(GLOB TEST_FILES
src/sort/*.cpp
)

add_executable(${PROJECT}_TEST test/main.test.cc ${TEST_FILES})
target_include_directories(${PROJECT}_TEST PRIVATE 
includes/
includes/imgui
includes/imgui/backends
includes/openal
)
target_link_libraries(${PROJECT}_TEST PRIVATE GTest::gtest_main)
target_compile_definitions(${PROJECT}_TEST PRIVATE TESTING)

gtest_discover_tests(${PROJECT}_TEST)
