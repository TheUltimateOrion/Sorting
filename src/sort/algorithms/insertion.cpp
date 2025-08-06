#include "sort/insertion/insertion.h"

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

#include "utils/common.h"

namespace Sort
{
    InsertionSort::InsertionSort() : BaseSort() { }

    void InsertionSort::sort()
    {
        for (std::size_t i = 1; i < elems.size(); ++i)
        {
            std::size_t j    = i;
            elem_t      temp = elems[i];

            while (j > 0 && elems[j - 1] > temp)
            {
                elems[j] = elems[j - 1];
                m_first  = j;
                m_second = j - 1;
                --j;

                Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
                RETURN_IF_STOPPED();
            }

            elems[j] = temp;
            m_first  = j;
            m_second = i;

            Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
            RETURN_IF_STOPPED();
        }
    }
}  // namespace Sort
