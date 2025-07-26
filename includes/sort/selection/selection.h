#pragma once
#include "sort/base.h"

namespace Sort 
{
    class SelectionSort : virtual public BaseSort
    {
    public:
        SelectionSort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort
