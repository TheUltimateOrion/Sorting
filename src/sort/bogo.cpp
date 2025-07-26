#include "sort/distribution/bogo.h"

#include <random>

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

namespace Sort {
    BogoSort::BogoSort(std::vector<int>& t_arr) : BaseSort(t_arr) {}

    bool BogoSort::isSorted()
    {
        if (elems.empty()) 
        {
            return true;
        }

        for (size_t i = 1; i < elems.size(); ++i) 
        {
            if (elems[i] < elems[i - 1]) 
            {
                return false;
            }
        }

        return true;
    }

    void BogoSort::bogoShuffle()
    {
        size_t n = elems.size();

        for (size_t i = 0; i < n; ++i) 
        {
            int randInt = rand() % n;
            swap(elems, i, randInt);
            if (wantClose || wantStop) return;
        }
    }

    void BogoSort::sort()
    {
        isSorting = true;
        
        if (elems.empty()) 
        {
            isSorting = false;
            sorted = true;
            return;
        }

        while(!isSorted()) 
        {
            bogoShuffle();
            if (wantClose || wantStop) return;
        }
        
        isSorting = false;
        sorted = true;
    }
} // namespace Sort
