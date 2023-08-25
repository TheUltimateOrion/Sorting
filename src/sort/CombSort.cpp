#include "sort/CombSort.h"

CombSort::CombSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

int CombSort::getNextGap(int gap)
{
    gap = (gap*10)/13;
    if (gap < 1)
        return 1;
    return gap;
}

void CombSort::sort()
{
    // Initialize gap
    int gap = elems.capacity();
 
    // Initialize swapped as true to make sure that
    // loop runs
    bool swapped = true;
 
    // Keep running while gap is more than 1 and last
    // iteration caused a swap
    while (gap != 1 || swapped == true)
    {
        // Find next gap
        gap = getNextGap(gap);
 
        // Initialize swapped as false so that we can
        // check if swap happened or not
        swapped = false;
 
        // Compare all elements with current gap
        for (int i = 0; i < elems.capacity() - gap; i++)
        {
            if (elems[i] > elems[i + gap])
            {
                SortRenderer::render(this, this->elems, i, i + gap);
                if (wantBreak)
                    return;
                swap(elems, i, i + gap);
                swapped = true;
            }
        }
    }
}