#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class RadixLSDSort : virtual public Sort
{
private:
    int radix;
    void countSortByDigits(int exponent, int minValue);
public:
    RadixLSDSort(std::vector<int>& arr, int radix);

    void sort() override;
};