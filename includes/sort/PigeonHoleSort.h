#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class PigeonHoleSort : virtual public Sort
{
public:
    PigeonHoleSort(std::vector<int>& arr, ImGuiIO* io);

    void sort() override;
};