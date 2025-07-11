#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class QuickSort : virtual public Sort
{
private:
    int partition(int t_low, int t_high);
    void quickSort(int t_low, int t_high);
public:
    QuickSort(std::vector<int>& t_arr);

    void sort() override;
};