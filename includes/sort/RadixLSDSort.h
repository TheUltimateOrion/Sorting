#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class RadixLSDSort : virtual public Sort
{
private:
    int radix;
public:
    RadixLSDSort(std::vector<int>& arr, ImGuiIO& io, int radix);

    void sort() override;
};