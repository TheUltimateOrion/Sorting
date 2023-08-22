#pragma once
#include "Sort.h"

class BubbleSort : public Sort
{
public:
    BubbleSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};