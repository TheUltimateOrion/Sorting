#pragma once

#include <vector>

#include "render_params.h"
#include "sort/base.h"
#include "state.h"

namespace Core
{
    class App;
}

namespace Renderer
{
    class SortView
    {
    private:
        std::weak_ptr<Core::App> m_app {};
        Sort::SortArray<int>     m_elems {};
        UIState&                 m_uiState;

        void                     drawElement(std::size_t t_index, RenderParams const& t_params) noexcept;

    public:
        SortView(std::shared_ptr<Core::App> t_app, UIState& t_state) noexcept;
        ~SortView() noexcept = default;

        void update(UIState& t_uiState) noexcept;
    };
}  // namespace Renderer
