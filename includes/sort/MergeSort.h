#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class MergeSort : virtual  public Sort
{
private:
    void merge(int const left, int const mid, int const right);
    void _mergeSort(int const begin, int const end);
public:
    MergeSort(std::vector<int>& arr);

    void sort() override;
};