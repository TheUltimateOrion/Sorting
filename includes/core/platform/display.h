#pragma once

#include <SDL3/SDL.h>

namespace Core::Platform::Display
{
    SDL_DisplayID          getCurrentDisplayID();
    const SDL_DisplayMode* getDisplayMode(SDL_DisplayID id);
    float                  getFramerate(void);
} // namespace Core::Platform::Display