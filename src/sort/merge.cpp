#include "sort/merge/merge.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort {
    MergeSort::MergeSort() : BaseSort() {}

    void MergeSort::merge(int const left, int const mid, int const right)
    {
        const int subArrayOne = mid - left + 1;
        const int subArrayTwo = right - mid;
    
        // Create temp arrays
        std::vector<int> leftArray(subArrayOne), rightArray(subArrayTwo);
    
        // Copy data to temp arrays leftArray[] and rightArray[]
        std::copy(elems.begin() + left, elems.begin() + mid + 1, leftArray.begin());
        std::copy(elems.begin() + mid + 1, elems.begin() + right + 1, rightArray.begin());
    
        int indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0, indexOfMergedArray = left;
    
        // Merge the temp arrays back into array[left..right]
        while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) 
        {
            if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) 
            {
                elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                m_first = indexOfMergedArray;
                m_second = indexOfSubArrayOne + left;
                
                indexOfSubArrayOne++;
            } 
            else 
            {
                elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                m_first = indexOfMergedArray;
                m_second = mid + indexOfSubArrayTwo + 1;
                
                if (wantClose || wantStop) return;
                indexOfSubArrayTwo++;
            }
                
            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            if (wantClose || wantStop) return;

            elems.getComparisons()++;
            indexOfMergedArray++;
        }
    
        // Copy the remaining elements of
        // left[], if there are any
        while (indexOfSubArrayOne < subArrayOne) 
        {
            elems[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            m_first = indexOfMergedArray;
            m_second = indexOfSubArrayOne + left;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            if (wantClose || wantStop) return;

            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }
    
        // Copy the remaining elements of
        // right[], if there are any
        while (indexOfSubArrayTwo < subArrayTwo) 
        {
            elems[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            m_first = indexOfMergedArray;
            m_second = mid + indexOfSubArrayTwo + 1;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            if (wantClose || wantStop) return;
            
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }
    }
    
    void MergeSort::mergeSort(int const begin, int const end)
    {
        if (begin >= end)
            return;
    
        int mid = begin + (end - begin) / 2;
        mergeSort(begin, mid);
        if (wantClose || wantStop) return;
        mergeSort(mid + 1, end);
        if (wantClose || wantStop) return;
        merge(begin, mid, end);
        if (wantClose || wantStop) return;
    }

    // begin is for left index and end is right index
    // of the sub-array of arr to be sorted
    void MergeSort::sort()
    {
        isSorting = true;
        int begin = 0;
        int end = elems.size() - 1;

        mergeSort(begin ,end);
        if (wantClose || wantStop) return;
        isSorting = false;
        sorted = true;
    }
} // namespace Sort
