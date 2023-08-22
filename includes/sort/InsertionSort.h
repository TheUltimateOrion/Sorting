#pragma once
#include "Sort.h"

class InsertionSort : public Sort
{
public:
    InsertionSort(std::vector<int>& arr, ImGuiIO& io);

    void sort(float speed = 1) override;
};