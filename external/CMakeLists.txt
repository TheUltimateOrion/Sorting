
add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(openal-soft EXCLUDE_FROM_ALL)

set(INSTALL_GTEST OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(googletest)
set(BUILD_SHARED_LIBS ON)
