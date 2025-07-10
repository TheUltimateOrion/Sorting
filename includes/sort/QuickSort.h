#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class QuickSort : virtual public Sort
{
private:
    int partition(std::vector<int>& arr, int low, int high);
    void quickSort(std::vector<int>& arr, int low, int high);
public:
    QuickSort(std::vector<int>& arr);

    void sort() override;
};