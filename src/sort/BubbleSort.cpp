#include "sort/BubbleSort.h"

BubbleSort::BubbleSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void BubbleSort::sort()
{
    isSorting = true;
    for (int i = elems.size() - 1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (elems[j] > elems[j + 1])
            {
                SortRenderer::render(this, this->elems, j, j + 1);
                if (wantBreak)
                    return;
                //std::cout << elems[j] << ": " << elems[j + 1] << std::endl;
                this->swap(elems, j, j + 1);
            }
        }
    }
    isSorting = false;
    sorted = true;
}