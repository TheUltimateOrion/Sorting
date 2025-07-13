#pragma once
#include "sort/base.h"

class BogoSort : virtual public BaseSort
{
private:
    bool isSorted();
    void bogoShuffle();
public:
    BogoSort(std::vector<int>& t_arr);

    void sort() override;
};