#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class CombSort : virtual public Sort
{
private:
    int getNextGap(int gap);
public:
    CombSort(std::vector<int>& arr);

    void sort() override;
};