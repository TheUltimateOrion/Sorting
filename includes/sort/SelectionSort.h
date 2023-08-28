#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class SelectionSort : virtual public Sort
{
public:
    SelectionSort(std::vector<int>& arr, ImGuiIO* io);

    void sort() override;
};