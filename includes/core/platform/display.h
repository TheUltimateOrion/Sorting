#pragma once

#include <SDL3/SDL.h>

namespace Core::Platform::Display
{
    SDL_DisplayID          getCurrentDisplayID();
    SDL_DisplayMode const* getDisplayMode(SDL_DisplayID t_id);
    float                  getFramerate();
}  // namespace Core::Platform::Display
