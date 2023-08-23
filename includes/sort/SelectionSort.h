#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class SelectionSort : public Sort
{
public:
    SelectionSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};