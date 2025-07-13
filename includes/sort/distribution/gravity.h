#pragma once
#include "sort/base.h"

class GravitySort : virtual public BaseSort
{
public:
    GravitySort(std::vector<int>& t_arr);

    void sort() override;
};