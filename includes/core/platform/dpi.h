#pragma once

#include <SDL3/SDL.h>

namespace Core::Platform 
{
    struct DPI 
    {
        float scaleX;
        float scaleY;

        DPI() noexcept;
        ~DPI() noexcept = default;

        static DPI from(SDL_Window *window, SDL_Renderer *renderer);
    };
}