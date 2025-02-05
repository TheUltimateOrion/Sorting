#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class CombSort : virtual public Sort
{
private:
    int getNextGap(int gap);
public:
    CombSort(std::vector<int>& arr);

    void sort() override;
};