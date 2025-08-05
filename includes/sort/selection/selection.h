#pragma once

#include "sort/base.h"

namespace Sort
{
    class SelectionSort : virtual public BaseSort
    {
    public:
        SelectionSort();

        void sort() override;
    };
}  // namespace Sort
