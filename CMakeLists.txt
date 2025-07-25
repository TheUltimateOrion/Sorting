cmake_minimum_required(VERSION 3.28)

project(Sorting VERSION 1.2.1)

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
)


target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf OpenAL::OpenAL)

# Install rules per platform
if(WIN32)
    install(
        TARGETS 
        ${PROJECT_NAME} 
        SDL3-shared 
        SDL3_ttf-shared
        OpenAL
        RUNTIME_DEPENDENCY_SET runtime_set
        RUNTIME DESTINATION .
    )

    message(STATUS "MSYSTEM PREFIX: $ENV{MSYSTEM_PREFIX}")
    install(RUNTIME_DEPENDENCY_SET runtime_set 
    
    DIRECTORIES
            "C:/msys64/ucrt64/bin"  # Adjust path as needed
            "C:/msys64/mingw64/bin"  # Adjust path as needed
            "$ENV{MSYSTEM_PREFIX}/bin"  # Adjust path as needed

    PRE_INCLUDE_REGEXES "lib.*\\.dll"

    # Exclude everything else (safe fallback)
    PRE_EXCLUDE_REGEXES
        ".*" 
    
    DESTINATION .)

    # install(FILES $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> DESTINATION .)

    

   
elseif(APPLE OR LINUX)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
    install(TARGETS SDL3-shared SDL3_ttf-shared OpenAL LIBRARY DESTINATION .)
    
    
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