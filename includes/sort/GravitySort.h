#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class GravitySort : public Sort
{
public:
    GravitySort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 10) override;
};