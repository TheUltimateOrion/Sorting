#pragma once

#include "core/registry/sort_registry.h"
#include "utils/common.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>

#include <cstdint>

namespace Renderer
{
    struct RenderContext
    {
        SDL_Window*           window {nullptr};
        SDL_Renderer*         renderer {nullptr};
        TTF_Font*             font {nullptr};
        float                 winWidth {600.0};
        float                 winHeight {400.0f};
        std::uint16_t         fps {60};

        static RenderContext* CreateContext(float width, float height, std::uint16_t fps);
        static void           DestroyContext(RenderContext* ctx);

        Utils::Signal         createFont(std::string const& t_relativePath);

        inline float          getFrameTime() const noexcept { return 1000.0f / fps; }

        ANDROID_API void      refreshOrientation();
    };
}  // namespace Renderer
