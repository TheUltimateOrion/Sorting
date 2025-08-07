#pragma once

#include "sort/base.h"
#include "sort/parametrized.h"

#include <cstdint>

namespace Sort
{
    class RadixLSDSort : virtual public BaseSort,
                         virtual public Parameterized<uint8_t>
    {
    private:
        void countSortByDigits(std::uint64_t t_exponent, elem_t t_minValue);

    public:
        RadixLSDSort();

        void sort() override;
    };
}  // namespace Sort
