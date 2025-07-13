#pragma once
#include "sort/base.h"

class SelectionSort : virtual public BaseSort
{
public:
    SelectionSort(std::vector<int>& t_arr);

    void sort() override;
};