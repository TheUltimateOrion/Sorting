#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class InsertionSort : virtual  public Sort
{
public:
    InsertionSort(std::vector<int>& arr);

    void sort() override;
};