#include "sort/distribution/radix_lsd.h"

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort
{
    RadixLSDSort::RadixLSDSort() : BaseSort {} { hasRadix = true; }

    void RadixLSDSort::countSortByDigits(std::uint64_t exponent, elem_t minValue)
    {
        std::uint8_t               bucketIndex;
        std::vector<std::uint32_t> buckets(m_radix);
        std::vector<elem_t>        output(elems.size());

        // Initialize bucket
        std::fill(buckets.begin(), buckets.end(), 0);

        // Count frequencies
        for (std::size_t i = 0; i < elems.size(); ++i)
        {
            bucketIndex = ((elems[i] - minValue) / exponent) % m_radix;
            buckets[bucketIndex]++;
        }

        // Compute cumulates
        for (std::size_t i = 1; i < m_radix; ++i) { buckets[i] += buckets[i - 1]; }

        // Move records
        for (ptrdiff_t i = elems.size() - 1; i >= 0; --i)
        {
            bucketIndex                    = ((elems[i] - minValue) / exponent) % m_radix;
            output[--buckets[bucketIndex]] = elems[i];
            m_first                        = i;
            m_second                       = bucketIndex;

            if (wantClose || wantStop) { return; }
        }

        // Copy back
        for (std::size_t i = 0; i < elems.size(); ++i)
        {
            elems[i] = output[i];
            m_first  = i;
            m_second = ((output[i] - minValue) / exponent) % m_radix;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            if (wantClose || wantStop) { return; }
        }
    }

    void RadixLSDSort::sort()
    {
        isSorting = true;

        if (elems.empty())
        {
            isSorting = false;
            sorted    = true;
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
            if (wantClose || wantStop) { return; }
            exponent *= m_radix;
        }

        isSorting = false;
        sorted    = true;
    }
}  // namespace Sort
