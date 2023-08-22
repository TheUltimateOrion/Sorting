#pragma once
#include "Sort.h"

class PigeonHoleSort : public Sort
{
public:
    PigeonHoleSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};