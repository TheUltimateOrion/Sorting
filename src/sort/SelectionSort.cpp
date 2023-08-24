#include "sort/SelectionSort.h"


SelectionSort::SelectionSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void SelectionSort::sort()
{
    isSorting = true;
    int size = LOGICAL_WIDTH;
    for (int i = 0; i < size - 1; i ++)
    {
        int min = i;
        for (int j = i + 1; j < size; j++)
        {
            if (elems[j] < elems[min])
                min = j;
        }
        SortRenderer::render(this, min, i);
        if (wantBreak)
            return;
        swap(elems, min, i);
    }
    isSorting = false;
    sorted = true;
}