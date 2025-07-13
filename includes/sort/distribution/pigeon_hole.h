#pragma once
#include "sort/base.h"

class PigeonHoleSort : virtual public BaseSort
{
public:
    PigeonHoleSort(std::vector<int>& t_arr);

    void sort() override;
};