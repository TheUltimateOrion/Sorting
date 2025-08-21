#pragma once

#include "core/registry/sort_registry.h"
#include "state.h"
#include "utils/common.h"
#include "utils/renderer.h"

#include <SDL3/SDL.h>

#include <array>
#include <memory>
#include <string>

namespace Renderer
{

    class UI
    {
    private:
        std::weak_ptr<Core::App>   m_app;

        UIState                    m_uiState {};

        std::array<char const*, 5> m_sortCategories {
            "Exchange", "Distribution", "Insertion", "Merge", "Select"
        };
        std::array<char const*, 8> m_sortDisplayTypes {"Bar",    "Dot",        "Rainbow Rectangle",
                                                       "Circle", "Circle Dot", "Disparity Circle",
                                                       "Spiral", "Spiral Dot"};

        void                       renderAboutWindow() noexcept;
        void                       renderDebugMenu();
        void                       renderText(
                                  std::string const& t_txt,
                                  float              t_x,
                                  float              t_y,
                                  SDL_Color          t_col
                              ) const noexcept;
        void renderInfo() const noexcept;

        void renderSortChooser(
            Core::SortRegistry const&       t_registry,
            std::string const&              t_currentName,
            std::vector<std::string> const& t_ids
        );
        void renderSortDisplayConfigs();
        void renderSortAlgorithmConfigs(Core::SortRegistryEntry const* const t_currentEntry);
        void renderActionButtons(
            Core::SortRegistryEntry const* const t_currentEntry,
            std::vector<std::string> const&      t_ids
        );

    public:
        UI(std::shared_ptr<Core::App> t_app) noexcept;
        ~UI() noexcept = default;

        void             renderUI();

        ANDROID_API void refreshOrientation();

        inline UIState&  getUIState() { return m_uiState; }
    };
}  // namespace Renderer
