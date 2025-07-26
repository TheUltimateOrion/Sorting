#include "sort/distribution/radix_lsd.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort {
    RadixLSDSort::RadixLSDSort(std::vector<int>& t_arr, int t_radix) : BaseSort(t_arr, true), m_radix(t_radix) {}

    void RadixLSDSort::countSortByDigits(int exponent, int minValue)
    {
        int bucketIndex;
        std::vector<int> buckets(m_radix);
        std::vector<int> output(elems.size());

        // Initialize bucket
        std::fill(buckets.begin(), buckets.end(), 0);
        
        // Count frequencies
        for (size_t i = 0; i < elems.size(); i++) 
        {
            bucketIndex = static_cast<int>(((elems[i] - minValue) / exponent) % m_radix);
            buckets[bucketIndex]++;
        }
        
        // Compute cumulates
        for (int i = 1; i < m_radix; i++) 
        {
            buckets[i] += buckets[i - 1];
        }

        // Move records
        for (int i = elems.size() - 1; i >= 0; i--) 
        {
            bucketIndex = static_cast<int>(((elems[i] - minValue) / exponent) % m_radix);
            output[--buckets[bucketIndex]] = elems[i];
            m_first = i;
            m_second = bucketIndex;

            if (wantClose || wantStop) return;
        }

        // Copy back
        for (size_t i = 0; i < elems.size(); i++) 
        {
            elems[i] = output[i];
            m_first = i;
            m_second = ((output[i] - minValue) / exponent) % m_radix;

            HIGH_RES_WAIT(1.f / BaseSort::s_speed);
            if (wantClose || wantStop) return;
        }
    }

    void RadixLSDSort::sort()
    {
        isSorting = true;

        if (elems.empty()) 
        {
            isSorting = false;
            sorted = true;
            return;
        }
        
        int minValue = elems[0];
        int maxValue = elems[0];
        for (size_t i = 1; i < elems.size(); i++) 
        {
            if (elems[i] < minValue) 
            {
                minValue = elems[i];
            } 
            else if (elems[i] > maxValue) 
            {
                    maxValue = elems[i];
            }
        }

        // Perform counting sort on each exponent/digit, starting at the least
        // significant digit
        int exponent = 1;

        while ((maxValue - minValue) / exponent >= 1) 
        {
            countSortByDigits(exponent, minValue);
            if (wantClose || wantStop) return;
            exponent *= m_radix;
        }
        
        isSorting = false;
        sorted = true;
    }
} // namespace Sort
