cmake_minimum_required(VERSION 3.28)

project(Sorting VERSION 1.2.0)

# Project-wide settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
set(SDLTTF_VENDORED ON)  # SDL_ttf builds its own dependencies

# Enable testing
include(CTest)
include(GoogleTest)
enable_testing()
add_subdirectory(test)

# External dependencies
add_subdirectory(external)

# Collect source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

# Define executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    includes/
    includes/imgui
    includes/imgui/backend
    includes/openal
)

# Link SDL and OpenAL per platform
if(WIN32 OR LINUX)
    set(OPENAL_LIB openal)
elseif(APPLE)
    set(OPENAL_LIB "-framework OpenAL")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf ${OPENAL_LIB})

# Install rules per platform
if(WIN32)
    install(TARGETS ${PROJECT_NAME} SDL3-shared SDL3_ttf-shared RUNTIME DESTINATION .)
elseif(APPLE OR LINUX)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
    install(TARGETS SDL3-shared SDL3_ttf-shared LIBRARY DESTINATION .)

    # Set proper RPATH
    set(RPATH "$ORIGIN")
    if(APPLE)
        set(RPATH "@executable_path")
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "${RPATH}"
        INSTALL_RPATH "${RPATH}"
    )
endif()

# Resource installation
install(DIRECTORY res/ DESTINATION res)

# Packaging metadata
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "orion")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sorting visualizer application")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)