cmake_minimum_required(VERSION 3.30.1)
set(PROJECT Sorting)
project(${PROJECT} VERSION 0.1.0)
include(CPack)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/dist)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
add_subdirectory(external)

add_executable(${PROJECT}_TEST tests/main.test.cc)
target_link_libraries(${PROJECT}_TEST PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT}_TEST)

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

if(WIN32)
    set(OPENAL_LIB openal)
    install(TARGETS ${PROJECT} SDL3-shared SDL3_ttf-shared RUNTIME DESTINATION .)
elseif(APPLE)
    set(OPENAL_LIB "-framework OpenAL")
    install(TARGETS ${PROJECT} RUNTIME DESTINATION .)
    install(TARGETS SDL3-shared SDL3_ttf-shared LIBRARY DESTINATION .)
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