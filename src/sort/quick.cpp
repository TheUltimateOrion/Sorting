#include "sort/exchange/quick.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

namespace Sort {
    QuickSort::QuickSort(std::vector<int>& t_arr) : BaseSort(t_arr) {}

    int QuickSort::partition(int t_low, int t_high)
    {
        int pivot = elems[t_high];

        int i = (t_low - 1);

        for (int j = t_low; j <= t_high - 1; j++) 
        {
            if (elems[j] < pivot) 
            {
                i++;
                swap(elems, i, j);
            }

            if (wantClose || wantStop) return 0;
            comparisons++;
        }
        m_first = i + 1;
        m_second = t_high;

        swap(elems, i + 1, t_high);
        if (wantClose || wantStop) return 0;
        return (i + 1);
    }

    void QuickSort::sort()
    {
        isSorting = true;
        quickSort(0, elems.size() - 1);

        if (wantClose || wantStop) return;
        
        isSorting = false;
        sorted = true;
    }

    void QuickSort::quickSort(int t_low, int t_high)
    {
        if (wantClose || wantStop) return;
        
        if (t_low < t_high) 
        {
            int pi = partition(t_low, t_high);

            if (wantClose || wantStop) return;
            quickSort(t_low, pi - 1);
            
            if (wantClose || wantStop) return;
            quickSort(pi + 1, t_high);
        }
    }
} // namespace Sort