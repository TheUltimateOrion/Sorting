#include "sort/exchange/bubble.h"

#include <cstddef>

namespace Sort
{
    BubbleSort::BubbleSort() : BaseSort() { }

    void BubbleSort::sort()
    {
        for (std::size_t i = elems.size() - 1; i > 0; --i)
        {
            for (std::size_t j = 0; j < i; ++j)
            {
                if (elems[j] > elems[j + 1])
                {
                    swap(elems, j, j + 1);

                    RETURN_IF_STOPPED();
                }

                elems.addComparisons(1);
            }
        }
    }
}  // namespace Sort
