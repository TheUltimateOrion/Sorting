#pragma once
#include "sort/base.h"

class CombSort : virtual public BaseSort
{
private:
    int getNextGap(int t_gap);
public:
    CombSort(std::vector<int>& t_arr);

    void sort() override;
};