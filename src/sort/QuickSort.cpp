#include "sort/QuickSort.h"

QuickSort::QuickSort(std::vector<int>& arr, ImGuiIO& io) : Sort(arr, io) {}

int QuickSort::partition(std::vector<int>& arr, int low, int high, float speed)
{
    int pivot = arr[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            Renderer::render(this, i, j, speed);
            if (wantBreak)
                return 0;
            swap(arr, i, j);
        }
    }
    Renderer::render(this, i + 1, high, speed);
    if (wantBreak)
        return 0;
    swap(arr, i + 1, high);
    return (i + 1);
}

void QuickSort::sort(float speed)
{
    this->quickSort(elems, 0, elems.capacity() - 1, speed);
    if (wantBreak)
        return;
    sorted = true;
}

void QuickSort::quickSort(std::vector<int>& arr, int low, int high, float speed)
{
    if (low < high) {
        int pi = partition(arr, low, high, speed);

        quickSort(arr, low, pi - 1, speed);
        if (wantBreak)
            return;
        quickSort(arr, pi + 1, high, speed);
        if (wantBreak)
            return;
    }
}