#pragma once

#include "sort/base.h"

namespace Sort
{
    class QuickSort : virtual public BaseSort
    {
    private:
        std::size_t partition(std::size_t t_low, std::size_t t_high);
        void        quickSort(std::size_t t_low, std::size_t t_high);

    public:
        QuickSort();

        void sort() override;
    };
}  // namespace Sort
