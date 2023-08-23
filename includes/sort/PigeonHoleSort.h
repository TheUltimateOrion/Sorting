#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class PigeonHoleSort : public Sort
{
public:
    PigeonHoleSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};