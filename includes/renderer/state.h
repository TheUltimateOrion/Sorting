#pragma once

#include "sort/category.h"

namespace Renderer
{
    struct UIState
    {
        bool           isColored {false};
        bool           isReversed {false};
        std::size_t    arrayLength {512};
        std::uint8_t   radix {2};
        std::uint8_t   sortIndex {0};
        Sort::Category sortCategory {Sort::Category::Exchange};
    };
}  // namespace Renderer
