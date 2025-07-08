#include "sort/QuickSort.h"

QuickSort::QuickSort(std::vector<int>& arr) : Sort(arr) {}

int QuickSort::partition(std::vector<int>& arr, int low, int high)
{
    int pivot = arr[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
            // app->sortRenderer->update(i, j);
        }
        if (wantClose || wantStop) return 0;
        this->comparisions++;
    }
    this->first = i + 1;
    this->second = high;
    // app->sortRenderer->update(i + 1, high);
    swap(arr, i + 1, high);
    if (wantClose || wantStop) return 0;
    return (i + 1);
}

void QuickSort::sort()
{
    isSorting = true;
    this->quickSort(elems, 0, elems.size() - 1);

    if (wantClose || wantStop)
        return;
    
    isSorting = false;
    sorted = true;
}

void QuickSort::quickSort(std::vector<int>& arr, int low, int high)
{
    if (wantClose || wantStop) return;
    
    if (low < high) {
        int pi = partition(arr, low, high);

        if (wantClose || wantStop) return;
        quickSort(arr, low, pi - 1);
        if (wantClose || wantStop) return;
        quickSort(arr, pi + 1, high);
    }
}