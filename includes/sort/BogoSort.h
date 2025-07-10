#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class BogoSort : virtual public Sort
{
private:
    bool isSorted(std::vector<int>& elems, int n);
    void bogoShuffle(std::vector<int>& elems, int n);
public:
    BogoSort(std::vector<int>& arr);

    void sort() override;
};