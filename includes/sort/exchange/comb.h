#pragma once
#include "sort/base.h"

namespace Sort 
{
    class CombSort : virtual public BaseSort
    {
    private:
        int getNextGap(int t_gap);
    public:
        CombSort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort
