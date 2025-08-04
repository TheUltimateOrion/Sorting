#pragma once

#include "sort/base.h"

namespace Sort 
{
    class BogoSort : virtual public BaseSort
    {
    private:
        bool isSorted();
        void bogoShuffle();
    public:
        BogoSort();

        void sort() override;
    };
} // namespace Sort
