#pragma once
#include "sort/base.h"

namespace Sort 
{
    class GravitySort : virtual public BaseSort
    {
    public:
        GravitySort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort
