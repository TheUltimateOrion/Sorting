#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class CombSort : virtual public Sort
{
private:
    int getNextGap(int t_gap);
public:
    CombSort(std::vector<int>& t_arr);

    void sort() override;
};