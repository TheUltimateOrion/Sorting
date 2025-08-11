#include "sort/merge/merge.h"

#include <algorithm>
#include <vector>

namespace Sort
{
    MergeSort::MergeSort() : BaseSort() { }

    void MergeSort::merge(std::size_t t_left, std::size_t t_mid, std::size_t t_right)
    {
        std::size_t         subArrayOne = t_mid - t_left + 1;
        std::size_t         subArrayTwo = t_right - t_mid;

        // Create temp arrays
        std::vector<elem_t> leftArray(subArrayOne), rightArray(subArrayTwo);

        // Copy data to temp arrays leftArray[] and rightArray[]
        std::copy(elems.begin() + t_left, elems.begin() + t_mid + 1, leftArray.begin());
        std::copy(elems.begin() + t_mid + 1, elems.begin() + t_right + 1, rightArray.begin());

        std::size_t indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0, indexOfMergedArray = t_left;

        // Merge the temp arrays back into array[t_left..t_right]
        while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
        {
            if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
            {
                elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                m_first                   = indexOfMergedArray;
                m_second                  = indexOfSubArrayOne + t_left;

                indexOfSubArrayOne++;
            }
            else
            {
                elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                m_first                   = indexOfMergedArray;
                m_second                  = t_mid + indexOfSubArrayTwo + 1;

                indexOfSubArrayTwo++;
            }

            elems.incComparisons();
            indexOfMergedArray++;

            Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);

            RETURN_IF_STOPPED();
        }

        // Copy the remaining elements of
        // t_left[], if there are any
        while (indexOfSubArrayOne < subArrayOne)
        {
            elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            m_first                   = indexOfMergedArray;
            m_second                  = indexOfSubArrayOne + t_left;

            indexOfSubArrayOne++;
            indexOfMergedArray++;

            Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);

            RETURN_IF_STOPPED();
        }

        // Copy the remaining elements of
        // t_right[], if there are any
        while (indexOfSubArrayTwo < subArrayTwo)
        {
            elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            m_first                   = indexOfMergedArray;
            m_second                  = t_mid + indexOfSubArrayTwo + 1;

            indexOfSubArrayTwo++;
            indexOfMergedArray++;

            Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);

            RETURN_IF_STOPPED();
        }
    }

    void MergeSort::mergeSort(std::size_t t_begin, std::size_t t_end)
    {
        if (t_begin >= t_end)
        {
            return;
        }

        std::size_t t_mid = t_begin + (t_end - t_begin) / 2;

        mergeSort(t_begin, t_mid);
        mergeSort(t_mid + 1, t_end);

        merge(t_begin, t_mid, t_end);
    }

    // begin is for t_left index and end is t_right index
    // of the sub-array of arr to be sorted
    void MergeSort::sort()
    {
        std::size_t begin = 0;
        std::size_t end   = elems.size() - 1;

        mergeSort(begin, end);
    }
}  // namespace Sort
