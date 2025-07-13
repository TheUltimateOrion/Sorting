#pragma once
#include "sort/base.h"

class BubbleSort : virtual public BaseSort
{
public:
    BubbleSort(std::vector<int>& t_arr);

    void sort() override;
};