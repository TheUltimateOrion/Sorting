#include "sort/BubbleSort.h"

BubbleSort::BubbleSort(std::vector<int>& arr) : Sort(arr) {}

void BubbleSort::sort()
{
    isSorting = true;
    for (int i = elems.size() - 1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (elems[j] > elems[j + 1])
            {
                //std::cout << elems[j] << ": " << elems[j + 1] << std::endl;
                swap(elems, j, j + 1);
                app->sortRenderer->update(elems, j, j + 1);
                if (wantClose || wantStop)
                    return;
            }
            comparisions++;
        }
    }
    isSorting = false;
    sorted = true;
}