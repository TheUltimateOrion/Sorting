#pragma once

#include "sort/base.h"

namespace Sort
{
    class PigeonHoleSort : virtual public BaseSort
    {
    public:
        PigeonHoleSort();

        void sort() override;
    };
}  // namespace Sort
