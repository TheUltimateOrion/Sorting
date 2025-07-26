#include "sort/exchange/comb.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

namespace Sort {
    CombSort::CombSort(std::vector<int>& t_arr) : BaseSort(t_arr) {}

    int CombSort::getNextGap(int t_gap)
    {
        int gap = (t_gap * 10) / 13;
        if (gap < 1)
        {
            return 1;
        }
        
        return gap;
    }

    void CombSort::sort()
    {
        isSorting = true;

        if(elems.empty()) 
        {
            isSorting = false;
            sorted = true;
            return;
        }
        
        // Initialize gap
        int gap = elems.size();
    
        // Initialize swapped as true to make sure that
        // loop runs
        bool swapped = true;
    
        // Keep running while gap is more than 1 and last
        // iteration caused a swap
        while (gap != 1 || swapped == true)
        {
            // Find next gap
            gap = getNextGap(gap);
            if (wantClose || wantStop) return;
    
            // Initialize swapped as false so that we can
            // check if swap happened or not
            swapped = false;
    
            // Compare all elements with current gap
            for (size_t i = 0; i < elems.size() - gap; i++)
            {
                if (elems[i] > elems[i + gap])
                {
                    swap(elems, i, i + gap);
                    if (wantClose || wantStop) return;
                    swapped = true;
                }
                comparisons++;
            }
        }
        isSorting = false;
        sorted = true;
    }
} // namespace Sort
