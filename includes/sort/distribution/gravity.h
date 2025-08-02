#pragma once
#include "sort/base.h"

namespace Sort 
{
    class GravitySort : virtual public BaseSort
    {
    public:
        GravitySort();

        void sort() override;
    };
} // namespace Sort
