#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class GravitySort : virtual public Sort
{
public:
    GravitySort(std::vector<int>& arr);

    void sort() override;
};