#pragma once
#include <memory>
#include <string>

#include <SDL3/SDL.h>


#include "utils/common.h"

namespace Core 
{
    class App;
}
namespace Renderer 
{
    class UI 
    {
    private:
        std::weak_ptr<Core::App> m_app;

        int m_arrayLength;
    public:
        UI(std::shared_ptr<Core::App> app) noexcept;
        ~UI() noexcept = default;

        void renderText(const std::string& t_txt, float t_x, float t_y, SDL_Color t_col) const noexcept;
        void renderInfo() const noexcept;
        Utils::Signal renderUI();
    };
} // namespace Renderer
