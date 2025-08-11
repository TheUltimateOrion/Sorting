#include "sort/exchange/quick.h"

namespace Sort
{
    QuickSort::QuickSort() : BaseSort() { }

    std::size_t QuickSort::partition(std::size_t t_low, std::size_t t_high)
    {
        elem_t         pivot = elems[t_high];

        // Only use signed where subtraction can go negative
        std::ptrdiff_t i     = static_cast<std::ptrdiff_t>(t_low) - 1;

        for (std::size_t j = t_low; j < t_high; ++j)
        {
            if (elems[j] < pivot)
            {
                ++i;
                swap(elems, static_cast<std::size_t>(i), j);
            }

            elems.addComparisons(1);
            RETURN_IF_STOPPED(0);
        }

        std::size_t const ip1 = static_cast<std::size_t>(i + 1);
        m_first               = ip1;
        m_second              = t_high;

        swap(elems, ip1, t_high);
        return ip1;
    }

    void QuickSort::sort()
    {
        if (elems.empty())
        {
            return;
        }
        quickSort(0, elems.size() - 1);
    }

    void QuickSort::quickSort(std::size_t t_low, std::size_t t_high)
    {
        if (t_low >= t_high)
        {
            return;
        }

        std::size_t pi = partition(t_low, t_high);

        RETURN_IF_STOPPED();
        if (pi > t_low)
        {
            quickSort(t_low, pi - 1);
        }

        RETURN_IF_STOPPED();
        if (pi < t_high)
        {
            quickSort(pi + 1, t_high);
        }
    }
}  // namespace Sort
