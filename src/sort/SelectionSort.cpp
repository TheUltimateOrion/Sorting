#include "sort/SelectionSort.h"


SelectionSort::SelectionSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void SelectionSort::sort()
{
    isSorting = true;
    int size = elems.size();
    for (int i = 0; i < size - 1; i ++)
    {
        int min = i;
        for (int j = i + 1; j < size; j++)
        {
            if (elems[j] < elems[min])
                min = j;
            comparisions++;
        }
        swap(elems, min, i);
        SortRenderer::render(this, this->elems, min, i);
        if (wantClose || wantStop)
            return;
    }
    isSorting = false;
    sorted = true;
}