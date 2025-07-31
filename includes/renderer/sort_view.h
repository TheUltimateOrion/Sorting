#pragma once
#include <vector>

#include "sort/base.h"
#include "render_params.h"

namespace Core
{
    class App;
}

namespace Renderer
{
    class SortView
    {
    private:
        std::weak_ptr<Core::App> m_app;
        std::vector<int> m_elems;

        void drawElement(size_t k, const RenderParams& t_params) noexcept;
    public:
        SortView(std::shared_ptr<Core::App> app) noexcept;
        ~SortView() noexcept = default;

        void update() noexcept;

        int renderGUI() noexcept;
    };
} // namespace Renderer
