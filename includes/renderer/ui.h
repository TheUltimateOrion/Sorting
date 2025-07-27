#pragma once
#include <string>
#include <SDL3/SDL.h>

#include "utils/common.h"

namespace Renderer 
{
    class UI 
    {
    public:
        UI() noexcept = default;
        ~UI() noexcept = default;

        void renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept;
        void renderInfo() const noexcept;
        Utils::Signal renderUI();
    };
} // namespace Renderer
