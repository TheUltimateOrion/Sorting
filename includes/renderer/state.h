#pragma once

#include "disp_type.h"
#include "sort/category.h"

#include <cstddef>
#include <cstdint>

namespace Renderer
{
    struct UIState
    {
        std::size_t    arrayLength {512};
        bool           isDebugMode {false};
        bool           isColored {false};
        bool           isImGuiOpen {true};
        bool           isReversed {false};
        bool           isAboutPopupOpened {false};
        bool           isScreenOrientationChanged {false};
        std::int64_t   sortParameter {1};
        Sort::Category sortCategory {Sort::Category::Exchange};
        DisplayType    sortDisplayType {DisplayType::Bar};
        std::uint8_t   sortIndex {0};
    };
}  // namespace Renderer
