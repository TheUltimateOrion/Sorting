#include "sort/distribution/bogo.h"

#include <random>

#ifndef TESTING
    #include "renderer/sort_view.h"
#endif

namespace Sort
{
    BogoSort::BogoSort() : BaseSort() { }

    bool BogoSort::isSorted()
    {
        if (elems.empty())
        {
            return true;
        }

        for (std::size_t i = 1; i < elems.size(); ++i)
        {
            if (elems[i] < elems[i - 1])
            {
                return false;
            }
        }

        return true;
    }

    void BogoSort::bogoShuffle()
    {
        std::size_t                      n = elems.size();

        std::random_device               rd {};
        static thread_local std::mt19937 gen(rd());

        for (std::size_t i = 0; i < n; ++i)
        {
            std::uniform_int_distribution<std::size_t> dist(0, n - 1);
            std::size_t                                randIndex = dist(gen);
            swap(elems, i, randIndex);
            RETURN_IF_STOPPED();
        }
    }

    void BogoSort::sort()
    {
        if (elems.empty())
        {
            m_flags.doneSorting();
            return;
        }

        while (!isSorted())
        {
            bogoShuffle();
            RETURN_IF_STOPPED();
        }
    }
}  // namespace Sort
