#include "sort/insertion/insertion.h"

#ifndef TESTING
#include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort {
    InsertionSort::InsertionSort() : BaseSort() {}

    void InsertionSort::sort()
    {
        isSorting = true;
        int size = elems.size();
        for (int i = 1; i < size; i++)
        {
            int j = i;
            int temp = elems[i];
            while (j > 0 && elems[j - 1] > temp)
            {
                elems[j] = elems[j - 1];
                m_first = j;
                m_second = j - 1;
                Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
                if (wantClose || wantStop) return;
                --j;
            }

            elems[j] = temp;
            m_first = j;
            m_second = i;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            if (wantClose || wantStop) return;
        }
        
        isSorting = false;
        sorted = true;
    }
} // namespace Sort
