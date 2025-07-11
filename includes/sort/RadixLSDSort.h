#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class RadixLSDSort : virtual public Sort
{
private:
    int m_radix;
    void countSortByDigits(int t_exponent, int t_minValue);
public:
    RadixLSDSort(std::vector<int>& t_arr, int t_radix);

    void sort() override;
};