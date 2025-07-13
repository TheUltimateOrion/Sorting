#pragma once
#include "sort/base.h"

class InsertionSort : virtual public BaseSort
{
public:
    InsertionSort(std::vector<int>& t_arr);

    void sort() override;
};