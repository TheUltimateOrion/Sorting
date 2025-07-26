#pragma once
#include <vector>

#include "sort/base.h"
#include "render_params.h"

namespace Renderer
{
    class SortView
    {
    private:
        std::vector<int> m_elems;

        void drawElement(size_t k, const RenderParams& t_params) noexcept;
    public:
        SortView() = default;
        ~SortView() = default;

        void update() noexcept;

        int renderGUI() noexcept;
    };
} // namespace Renderer
