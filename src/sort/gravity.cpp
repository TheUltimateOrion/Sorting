#include "sort/distribution/gravity.h"

#include <algorithm>
#include <numeric>

#include "utils/common.h"

namespace Sort
{
    GravitySort::GravitySort() : BaseSort() { }

    void GravitySort::sort()
    {
        isSorting = true;

        if (elems.empty())
        {
            isSorting = false;
            sorted    = true;
            return;
        }

        elem_t max = *std::max_element(elems.begin(), elems.end());

        if (max <= 0)
        {
            isSorting = false;
            sorted    = true;
            return;
        }

        std::size_t                      n = elems.size();

        std::vector<std::vector<elem_t>> abacus(n, std::vector<elem_t>(max, 0));
        for (std::size_t i = 0; i < n; ++i)
        {
            for (elem_t j = 0; j < elems[i]; ++j)
            {
                abacus[i][max - j - 1] = 1;
            }
        }

        for (elem_t col = 0; col < max; ++col)
        {
            elem_t count = 0;

            for (std::size_t row = 0; row < n; ++row)
            {
                count += abacus[row][col];
            }

            for (std::ptrdiff_t row = n - 1; row >= 0; --row)
            {
                abacus[row][col] = (count-- > 0 ? 1 : 0);
            }

            for (std::size_t i = 0; i < n; ++i)
            {
                elems[i] = std::accumulate(abacus[i].begin(), abacus[i].end(), 0);
                m_first  = i;
                m_second = i;

                Core::Timer::sleep(1.f / BaseSort::s_speed, realTimer);
                if (wantClose || wantStop) { return; }
            }
        }

        isSorting = false;
        sorted    = true;
    }
}  // namespace Sort
