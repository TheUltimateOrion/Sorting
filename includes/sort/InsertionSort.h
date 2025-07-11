#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class InsertionSort : virtual  public Sort
{
public:
    InsertionSort(std::vector<int>& t_arr);

    void sort() override;
};