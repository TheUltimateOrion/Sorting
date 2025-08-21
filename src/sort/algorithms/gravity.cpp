#include "sort/distribution/gravity.h"

#include <algorithm>
#include <numeric>
#include <vector>

#include <cstddef>

namespace Sort
{
    GravitySort::GravitySort() : BaseSort() { }

    void GravitySort::sort()
    {
        elem_t max = *std::max_element(elems.begin(), elems.end());

        if (elems.empty() || max <= 0) { return; }

        std::size_t                      n = elems.size();

        std::vector<std::vector<elem_t>> abacus(n, std::vector<elem_t>(max, 0));
        for (std::size_t i = 0; i < n; ++i)
        {
            for (elem_t j = 0; j < elems[i]; ++j) { abacus[i][max - j - 1] = 1; }
        }

        for (elem_t col = 0; col < max; ++col)
        {
            elem_t count = 0;

            for (std::size_t row = 0; row < n; ++row) { count += abacus[row][col]; }

            for (std::ptrdiff_t row = n - 1; row >= 0; --row)
            {
                abacus[row][col] = (count-- > 0 ? 1 : 0);
            }

            for (std::size_t i = 0; i < n; ++i)
            {
                elems[i] = std::accumulate(abacus[i].begin(), abacus[i].end(), 0);
                m_first  = i;
                m_second = i;

                Core::Timer::Sleep(1.f / BaseSort::s_speed, realTimer);
                RETURN_IF_STOPPED();
            }
        }
    }
}  // namespace Sort
