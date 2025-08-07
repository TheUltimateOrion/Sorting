#include "sort/exchange/quick.h"

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

            elems.incComparisons();

            RETURN_IF_STOPPED(0);
        }

        m_first  = i + 1;
        m_second = t_high;

        swap(elems, static_cast<std::size_t>(i + 1), t_high);

        return static_cast<std::size_t>(i + 1);
    }

    void QuickSort::sort()
    {
        quickSort(0, elems.size() - 1);
    }

    void QuickSort::quickSort(std::size_t t_low, std::size_t t_high)
    {
        if (t_low < t_high)
        {
            std::size_t pi = partition(t_low, t_high);

            RETURN_IF_STOPPED();
            quickSort(t_low, pi - 1);

            RETURN_IF_STOPPED();
            quickSort(pi + 1, t_high);
        }
    }
}  // namespace Sort
