#include "sort/exchange/quick.h"

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

namespace Sort
{
    QuickSort::QuickSort() : BaseSort() { }

    std::size_t QuickSort::partition(std::size_t t_low, std::size_t t_high)
    {
        elem_t         pivot = elems[t_high];
        std::ptrdiff_t i     = static_cast<std::ptrdiff_t>(t_low) - 1;

        for (std::size_t j = t_low; j <= t_high - 1; ++j)
        {
            if (elems[j] < pivot)
            {
                ++i;
                swap(elems, static_cast<std::size_t>(i), j);
            }

            if (wantClose || wantStop)
            {
                return 0;
            }
            elems.incComparisons();
        }
        m_first  = i + 1;
        m_second = t_high;

        swap(elems, static_cast<std::size_t>(i + 1), t_high);
        if (wantClose || wantStop)
        {
            return 0;
        }
        return static_cast<std::size_t>(i + 1);
    }

    void QuickSort::sort()
    {
        isSorting = true;
        quickSort(0, elems.size() - 1);

        if (wantClose || wantStop)
        {
            return;
        }

        isSorting = false;
        sorted    = true;
    }

    void QuickSort::quickSort(std::size_t t_low, std::size_t t_high)
    {
        if (wantClose || wantStop) { return; }

        if (t_low < t_high)
        {
            std::size_t pi = partition(t_low, t_high);

            if (wantClose || wantStop) { return; }
            quickSort(t_low, pi - 1);

            if (wantClose || wantStop) { return; }
            quickSort(pi + 1, t_high);
        }
    }
}  // namespace Sort
