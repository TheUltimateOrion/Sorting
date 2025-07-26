#pragma once
#include "sort/base.h"

namespace Sort 
{
    class BubbleSort : virtual public BaseSort
    {
    public:
        BubbleSort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort
