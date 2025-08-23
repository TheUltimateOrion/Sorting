#include "sort/exchange/cocktail_shaker.h"

#include <cstddef>

namespace Sort
{
    CocktailShakerSort::CocktailShakerSort() : BaseSort() { }

    void CocktailShakerSort::sort()
    {
        bool swapped;
        do {
            swapped = false;
            for (std::size_t i = 0; i < elems.size() - 1; ++i)
            {
                if (elems[i] > elems[i + 1])
                {
                    swap(elems, i, i + 1);
                    swapped = true;

                    RETURN_IF_STOPPED();
                }
            }

            if (!swapped) { break; }

            swapped = false;

            for (std::size_t i = elems.size() - 2; i > 0; --i)
            {
                if (elems[i] > elems[i + 1])
                {
                    swap(elems, i + 1, i);
                    swapped = true;

                    RETURN_IF_STOPPED();
                }
            }
        }
        while (swapped);
    }
}  // namespace Sort
