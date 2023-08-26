#include "sort/QuickSort.h"

QuickSort::QuickSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

int QuickSort::partition(std::vector<int>& arr, int low, int high)
{
    int pivot = arr[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            SortRenderer::render(this, this->elems, i, j);
            if (wantBreak)
                return 0;
            swap(arr, i, j);
        }
    }
    SortRenderer::render(this, this->elems, i + 1, high);
    if (wantBreak)
        return 0;
    swap(arr, i + 1, high);
    return (i + 1);
}

void QuickSort::sort()
{
    isSorting = true;
    this->quickSort(elems, 0, elems.size());
    if (wantBreak)
        return;
    isSorting = false;
    sorted = true;
}

void QuickSort::quickSort(std::vector<int>& arr, int low, int high)
{
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        if (wantBreak)
            return;
        quickSort(arr, pi + 1, high);
        if (wantBreak)
            return;
    }
}