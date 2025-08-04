#pragma once

#include <chrono>
#include <memory>

#include "core/platform/dpi.h"
#include "registry/sort_registry.h"

namespace Core
{
    struct Ctx
    {
        Core::Platform::DPI dpi;
        SDL_Window*         window;
        SDL_Renderer*       renderer;
        float               winWidth  = 600.0f;
        float               winHeight = 400.0f;
        uint16_t            fps       = 60;

        static Ctx* createContext(float width, float height, uint16_t fps);
        static void destroyContext(Ctx* ctx);
        float       getFrameTime() const noexcept;
    };
} // namespace Core
