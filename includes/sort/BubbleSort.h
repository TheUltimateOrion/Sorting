#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class BubbleSort : virtual public Sort
{
public:
    BubbleSort(std::vector<int>& arr, ImGuiIO* io);

    void sort() override;
};