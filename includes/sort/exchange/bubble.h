#pragma once
#include "sort/base.h"

namespace Sort 
{
    class BubbleSort : virtual public BaseSort
    {
    public:
        BubbleSort();

        void sort() override;
    };
} // namespace Sort
