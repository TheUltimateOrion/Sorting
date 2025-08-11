#include "sort/distribution/pigeon_hole.h"

#include <vector>

#include <cstddef>

namespace Sort
{
    PigeonHoleSort::PigeonHoleSort() : BaseSort() { }

    void PigeonHoleSort::sort()
    {
        std::size_t size = elems.size();

        if (elems.empty())
        {
            return;
        }

        elem_t min = elems[0], max = elems[0];
        for (std::size_t i = 1; i < size; ++i)
        {
            if (elems[i] < min)
            {
                min = elems[i];
            }

            if (elems[i] > max)
            {
                max = elems[i];
            }

            m_first  = i;
            m_second = m_first.load();

            Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);

            RETURN_IF_STOPPED();
            elems.addComparisons(2);
        }

        std::vector<std::vector<elem_t>> holes(max - min + 1);

        for (std::size_t i = 0; i < size; ++i)
        {
            holes[elems[i] - min].push_back(elems[i]);
        }

        std::size_t index = 0;
        for (std::size_t i = 0; i < holes.size(); ++i)
        {
            for (std::size_t val : holes[i])
            {
                elems[index] = val;
                m_first      = index;
                m_second     = i;
                ++index;

                Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);
                RETURN_IF_STOPPED();
            }
        }
    }
}  // namespace Sort
