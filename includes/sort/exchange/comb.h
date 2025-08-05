#pragma once

#include "sort/base.h"

namespace Sort
{
    class CombSort : virtual public BaseSort
    {
    private:
        std::size_t getNextGap(std::size_t t_gap);

    public:
        CombSort();

        void sort() override;
    };
}  // namespace Sort
