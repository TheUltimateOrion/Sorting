#pragma once

#include "sort/base.h"

namespace Sort 
{
    class QuickSort : virtual public BaseSort
    {
    private:
        int partition(int t_low, int t_high);
        void quickSort(int t_low, int t_high);
    public:
        QuickSort();

        void sort() override;
    };
} // namespace Sort
