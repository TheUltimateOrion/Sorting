#pragma once

#include "sort/base.h"

namespace Sort
{
    class MergeSort : virtual public BaseSort
    {
    private:
        void merge(std::size_t t_left, std::size_t t_mid, std::size_t t_right);
        void mergeSort(std::size_t t_begin, std::size_t t_end);

    public:
        MergeSort();

        void sort() override;
    };
}  // namespace Sort
