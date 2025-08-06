#pragma once

#include "disp_type.h"
#include "sort/category.h"

namespace Renderer
{
    struct UIState
    {
        std::size_t    arrayLength {512};
        bool           isColored {false};
        bool           isImGuiOpen {true};
        bool           isReversed {false};
        std::uint8_t   radix {2};
        Sort::Category sortCategory {Sort::Category::Exchange};
        DisplayType    sortDisplayType {DisplayType::Bar};
        std::uint8_t   sortIndex {0};
    };
}  // namespace Renderer
