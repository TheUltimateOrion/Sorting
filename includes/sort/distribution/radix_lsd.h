#pragma once
#include "sort/base.h"

namespace Sort 
{
    class RadixLSDSort : virtual public BaseSort
    {
    private:
        int m_radix;
        void countSortByDigits(int t_exponent, int t_minValue);
    public:
        RadixLSDSort(std::vector<int>& t_arr, int t_radix);

        void sort() override;
    };
} // namespace Sort