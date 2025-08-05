#pragma once

#include "sort/base.h"

namespace Sort
{
    class InsertionSort : virtual public BaseSort
    {
    public:
        InsertionSort();

        void sort() override;
    };
}  // namespace Sort
