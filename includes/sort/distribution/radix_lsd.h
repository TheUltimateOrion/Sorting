#pragma once

#include "sort/base.h"

namespace Sort
{
    class RadixLSDSort : virtual public BaseSort
    {
    private:
        std::uint8_t m_radix {2};
        void         countSortByDigits(std::uint64_t t_exponent, elem_t t_minValue);

    public:
        RadixLSDSort();

        inline void setRadix(std::uint8_t t_radix) { m_radix = t_radix; }

        void        sort() override;
    };
}  // namespace Sort
