#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class BubbleSort : virtual public Sort
{
public:
    BubbleSort(std::vector<int>& arr);

    void sort() override;
};