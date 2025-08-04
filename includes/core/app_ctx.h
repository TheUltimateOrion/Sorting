#pragma once

#include <memory>
#include <chrono>

#include "core/platform/dpi.h"
#include "registry/sort_registry.h"

namespace Core
{
    struct Ctx 
    {
        SDL_Window *window;
        SDL_Renderer *renderer;

        static Ctx *createContext(float width, float height, uint16_t fps);
        static void destroyContext(Ctx *ctx);

        float winWidth = 600.0f;
        float winHeight = 400.0f;
        uint16_t fps = 60;

        Core::Platform::DPI dpi;
        
        float getFrameTime() const noexcept;
        
    };
}
