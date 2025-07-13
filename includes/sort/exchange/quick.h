#pragma once
#include "sort/base.h"

class QuickSort : virtual public BaseSort
{
private:
    int partition(int t_low, int t_high);
    void quickSort(int t_low, int t_high);
public:
    QuickSort(std::vector<int>& t_arr);

    void sort() override;
};