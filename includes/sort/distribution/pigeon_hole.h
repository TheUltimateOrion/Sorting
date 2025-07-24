#pragma once
#include "sort/base.h"

namespace Sort 
{
    class PigeonHoleSort : virtual public BaseSort
    {
    public:
        PigeonHoleSort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort