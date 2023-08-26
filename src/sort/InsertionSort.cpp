#include "sort/InsertionSort.h"

InsertionSort::InsertionSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void InsertionSort::sort()
{
    isSorting = true;
    int size = elems.size();
    for (int i = 1; i < size; i++)
    {
        int j = i;
        int temp = elems[i];
        while (j > 0 && elems[j - 1] > temp)
        {
            SortRenderer::render(this, this->elems, j, j - 1);
            if (wantBreak)
                return;
            elems[j] = elems[j - 1];
            --j;
        }
        elems[j] = temp;
    }
    isSorting = false;
    sorted = true;
}