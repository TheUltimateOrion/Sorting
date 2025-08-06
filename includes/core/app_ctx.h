#pragma once

#include <chrono>
#include <memory>

#include <SDL3_ttf/SDL_ttf.h>

#include "core/platform/dpi.h"
#include "registry/sort_registry.h"
#include "utils/common.h"

namespace Core
{
    struct Ctx
    {
        Platform::DPI dpi;
        SDL_Window*   window {nullptr};
        SDL_Renderer* renderer {nullptr};
        TTF_Font*     font {nullptr};
        float         winWidth {600.0};
        float         winHeight {400.0f};
        std::uint16_t fps {60};

        static Ctx*   createContext(float width, float height, std::uint16_t fps);
        static void   destroyContext(Ctx* ctx);

        Utils::Signal createFont(std::string const& relativePath);

        inline float  getFrameTime() const noexcept { return 1000.0f / fps; }
    };
}  // namespace Core
