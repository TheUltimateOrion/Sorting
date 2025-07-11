#pragma once
#include "Sort.h"

#ifndef TESTING
#include "../renderer/Renderer.h"
#endif

class PigeonHoleSort : virtual public Sort
{
public:
    PigeonHoleSort(std::vector<int>& t_arr);

    void sort() override;
};