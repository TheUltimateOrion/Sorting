#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class SelectionSort : virtual public Sort
{
public:
    SelectionSort(std::vector<int>& t_arr);

    void sort() override;
};