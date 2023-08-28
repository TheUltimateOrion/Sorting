#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class InsertionSort : virtual  public Sort
{
public:
    InsertionSort(std::vector<int>& arr, ImGuiIO* io);

    void sort() override;
};