#pragma once

#include <SDL3/SDL.h>

namespace Core::Platform
{
    struct DPI
    {
        float scaleX {1.0f};
        float scaleY {1.0f};

        DPI() noexcept  = default;
        ~DPI() noexcept = default;

        static DPI from(SDL_Window* t_window, SDL_Renderer* t_renderer);
    };
}  // namespace Core::Platform
