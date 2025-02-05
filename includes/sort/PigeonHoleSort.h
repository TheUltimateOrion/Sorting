#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class PigeonHoleSort : virtual public Sort
{
public:
    PigeonHoleSort(std::vector<int>& arr);

    void sort() override;
};