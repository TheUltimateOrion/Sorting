#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class BogoSort : virtual public Sort
{
private:
    bool isSorted();
    void bogoShuffle();
public:
    BogoSort(std::vector<int>& t_arr);

    void sort() override;
};