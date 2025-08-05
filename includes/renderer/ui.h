#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

#include "state.h"
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

        UIState                  m_uiState {};

    public:
        UI(std::shared_ptr<Core::App> t_app) noexcept;
        ~UI() noexcept = default;

        void renderText(
            std::string const& t_txt,
            float              t_x,
            float              t_y,
            SDL_Color          t_col
        ) const noexcept;

        void            renderInfo() const noexcept;
        Utils::Signal   renderUI();

        inline UIState& getUIState() { return m_uiState; }
    };
}  // namespace Renderer
