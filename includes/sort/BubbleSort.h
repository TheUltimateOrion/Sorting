#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class BubbleSort : virtual public Sort
{
public:
    BubbleSort(std::vector<int>& t_arr);

    void sort() override;
};