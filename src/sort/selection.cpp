#include "sort/selection/selection.h"

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort
{
    SelectionSort::SelectionSort() : BaseSort() { }

    void SelectionSort::sort()
    {
        isSorting        = true;
        std::size_t size = elems.size();

        for (std::size_t i = 0; i < size - 1; ++i)
        {
            std::size_t min = i;
            for (std::size_t j = i + 1; j < size; ++j)
            {
                m_first  = j;
                m_second = min;

                elems.incComparisons();

                if (elems[j] < elems[min])
                {
                    min = j;
                }

                Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
                if (wantClose || wantStop) { return; }
            }
            if (min != i)
            {
                swap(elems, min, i);
            }

            if (wantClose || wantStop) { return; }
        }

        isSorting = false;
        sorted    = true;
    }
}  // namespace Sort
