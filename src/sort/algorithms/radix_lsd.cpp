#include "sort/distribution/radix_lsd.h"

#include <algorithm>
#include <vector>

#include <cstddef>

namespace Sort
{
    RadixLSDSort::RadixLSDSort() : BaseSort {}
    {
        setParameterBounds(2, 10);
    }

    void RadixLSDSort::countSortByDigits(std::uint64_t exponent, elem_t minValue)
    {
        uint8_t                    radix = getParameter();

        std::uint8_t               bucketIndex;
        std::vector<std::uint32_t> buckets(radix);
        std::vector<elem_t>        output(elems.size());

        // Initialize bucket
        std::fill(buckets.begin(), buckets.end(), 0);

        // Count frequencies
        for (std::size_t i = 0; i < elems.size(); ++i)
        {
            bucketIndex = ((elems[i] - minValue) / exponent) % radix;
            buckets[bucketIndex]++;
        }

        // Compute cumulates
        for (std::size_t i = 1; i < radix; ++i) { buckets[i] += buckets[i - 1]; }

        // Move records
        for (ptrdiff_t i = elems.size() - 1; i >= 0; --i)
        {
            bucketIndex                    = ((elems[i] - minValue) / exponent) % radix;
            output[--buckets[bucketIndex]] = elems[i];
            m_first                        = i;
            m_second                       = bucketIndex;

            RETURN_IF_STOPPED();
        }

        // Copy back
        for (std::size_t i = 0; i < elems.size(); ++i)
        {
            elems[i] = output[i];
            m_first  = i;
            m_second = ((output[i] - minValue) / exponent) % radix;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            RETURN_IF_STOPPED();
        }
    }

    void RadixLSDSort::sort()
    {
        if (elems.empty())
        {
            m_flags.setFlags(FlagGroup::DoneSorting);
            return;
        }

        elem_t minValue = elems[0];
        elem_t maxValue = elems[0];

        for (std::size_t i = 1; i < elems.size(); ++i)
        {
            if (elems[i] < minValue) { minValue = elems[i]; }
            else if (elems[i] > maxValue) { maxValue = elems[i]; }
        }

        // Perform counting sort on each exponent/digit, starting at the least
        // significant digit
        std::uint64_t exponent = 1;

        while ((maxValue - minValue) / exponent >= 1)
        {
            countSortByDigits(exponent, minValue);
            RETURN_IF_STOPPED();
            exponent *= getParameter();
        }
    }
}  // namespace Sort
