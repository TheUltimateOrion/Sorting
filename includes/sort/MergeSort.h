#pragma once
#include "Sort.h"
#include "../renderer/Renderer.h"

class MergeSort : virtual  public Sort
{
private:
    void merge(int const left, int const mid, int const right);
    void _mergeSort(int const begin, int const end);
public:
    MergeSort(std::vector<int>& arr);

    void sort() override;
};