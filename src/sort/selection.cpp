#include "sort/selection/selection.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort {
    SelectionSort::SelectionSort(std::vector<int>& t_arr) : BaseSort(t_arr) {}

    void SelectionSort::sort()
    {
        isSorting = true;
        int size = elems.size();

        for (int i = 0; i < size - 1; i ++)
        {
            int min = i;
            for (int j = i + 1; j < size; j++)
            {
                m_first = j;
                m_second = min;

                comparisons++;

                if (elems[j] < elems[min])
                {
                    min = j;
                }

                HIGH_RES_WAIT(1.f / BaseSort::s_speed);
                if (wantClose || wantStop) return;

            }
            if (min != i)
            {
                swap(elems, min, i);
            }

            if (wantClose || wantStop) return;
        }

        isSorting = false;
        sorted = true;
    }
} // namespace Sort
