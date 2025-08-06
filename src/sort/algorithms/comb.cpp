#include "sort/exchange/comb.h"

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

namespace Sort
{
    CombSort::CombSort() : BaseSort() { }

    std::size_t CombSort::getNextGap(std::size_t t_gap)
    {
        std::size_t gap = (t_gap * 10) / 13;
        if (gap < 1)
        {
            return 1;
        }

        return gap;
    }

    void CombSort::sort()
    {
        if (elems.empty())
        {
            m_flags.doneSorting();
            return;
        }

        // Initialize gap
        std::size_t gap     = elems.size();

        // Initialize swapped as true to make sure that
        // loop runs
        bool        swapped = true;

        // Keep running while gap is more than 1 and last
        // iteration caused a swap
        while (gap != 1 || swapped == true)
        {
            // Find next gap
            gap = getNextGap(gap);
            RETURN_IF_STOPPED();

            // Initialize swapped as false so that we can
            // check if swap happened or not
            swapped = false;

            // Compare all elements with current gap
            for (std::size_t i = 0; i < elems.size() - gap; ++i)
            {
                if (elems[i] > elems[i + gap])
                {
                    swap(elems, i, i + gap);
                    swapped = true;

                    RETURN_IF_STOPPED();
                }
                elems.incComparisons();
            }
        }
    }
}  // namespace Sort
