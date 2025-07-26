#pragma once
#include "sort/base.h"

namespace Sort 
{
    class MergeSort : virtual public BaseSort
    {
    private:
        void merge(int const t_left, int const t_mid, int const t_right);
        void mergeSort(int const t_begin, int const t_end);
    public:
        MergeSort(std::vector<int>& t_arr);

        void sort() override;
    };
} // namespace Sort
