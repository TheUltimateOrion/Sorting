#pragma once

#include "sort/base.h"

namespace Sort
{
    class CocktailShakerSort : virtual public BaseSort
    {
    public:
        CocktailShakerSort();

        void sort() override;
    };
}  // namespace Sort
