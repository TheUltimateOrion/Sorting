#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class GravitySort : virtual public Sort
{
public:
    GravitySort(std::vector<int>& arr, ImGuiIO& io);

    void sort() override;
};