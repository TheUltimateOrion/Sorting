#include "sort/SelectionSort.h"


SelectionSort::SelectionSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

void SelectionSort::sort(float speed)
{
    int size = LOGICAL_WIDTH;
    for (int i = 0; i < size - 1; i ++)
    {
        int min = i;
        for (int j = i + 1; j < size; j++)
        {
            if (elems[j] < elems[min])
            {
                min = j;
            }
        }
        Renderer::render(this, min, i, speed);
        if (wantBreak)
            return;
        swap(elems, min, i);
    }
    sorted = true;
}